#include "CommandHandler.hpp"

#include <iostream>
#include <vector>
#include <stdexcept>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <boost/foreach.hpp>

#include "Server.hpp"
#include "Utils/Logger.hpp"
#include "Network/Protocol.hpp"

CommandHandler::~CommandHandler()
{
	for (auto& obj : m_commands)
		delete obj.second;
}

void CommandHandler::Register(const std::string name, ICommand* command)
{
	typedef std::map<std::string, ICommand*> CommandMap;
	std::pair<CommandMap::iterator, bool> res = m_commands.insert(std::make_pair(name, command));

	if (!res.second) {
		std::cout << "Command " << name << " already exists" << std::endl;
	} else {
		std::cout << "Registered command " << name << std::endl;
	}
}

void CommandHandler::Execute(Client* sender, std::string name, const CommandArgs& args)
{
	auto iter = m_commands.find(name);
	if (iter == m_commands.end()) {
		Server::SendWrappedMessage(sender, "&cUnknown command: " + name);
	} else {
		// Check if player has permission
		bool hasPermission = sender->GetUserType() >= iter->second->GetPermissionLevel();
		if (!hasPermission) {
			SendMessage(sender, "&cYou don't have permission to use that command.");
			return;
		}

		if (args.size() >= iter->second->GetArgumentAmount())
			iter->second->Execute(sender, args);
		else
			Server::SendWrappedMessage(sender, "&b" + iter->second->GetDocString());
	}
}

void CommandHandler::Handle(Client* sender, std::string command)
{
	std::vector<std::string> tokens;
	try {
		// Remove leading, trailing, and multiple whitespaces
		boost::trim_all(command);

		// Split command into tokens delimited by spaces
		boost::split(tokens, command, boost::is_any_of(" "));

		std::string commandName = tokens.front();

		boost::algorithm::to_lower(commandName);

		// Erase first element (command name), leaving arguments
		tokens.erase(tokens.begin());

		Execute(sender, commandName, tokens);
	} catch (std::runtime_error& e) {
		LOG(DEBUG, "%s", e.what());
	}
}
