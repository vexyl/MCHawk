#include "CommandHandler.hpp"

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

void CommandHandler::Register(std::string name, ICommand* command, std::string aliases)
{
	std::pair<CommandMap::iterator, bool> res = m_commands.insert(std::make_pair(name, command));

	if (!res.second) {
		LOG(LogLevel::kWarning, "Command %s already exists", name.c_str());
		return;
	}

	std::string commandString = name;

	if (!aliases.empty()) {
		commandString += " (";

		std::vector<std::string> aliasList;

		boost::split(aliasList, aliases, boost::is_any_of(" "));

		int size = (int)aliasList.size();
		for (int i = 0; i < size; ++i) {
			std::string alias = aliasList[i];

			std::pair<AliasMap::iterator, bool> res = m_aliases.insert(std::make_pair(alias, name));

			commandString += alias;

			if (!res.second)
				commandString += "[X]";

			if (i < size - 1)
				commandString += ", ";
			else
				commandString += ")";
		}
	}

	LOG(LogLevel::kDebug, "Registered command %s", commandString.c_str());
}

void CommandHandler::Execute(Client* sender, std::string name, const CommandArgs& args)
{
	auto iter = m_commands.find(name);

	// Not found, check aliases
	if (iter == m_commands.end()) {
		auto aliasIter = m_aliases.find(name);
		if (aliasIter != m_aliases.end())
			iter = m_commands.find(aliasIter->second);
	}

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
		LOG(LogLevel::kDebug, "%s", e.what());
	}
}
