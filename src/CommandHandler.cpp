#include "CommandHandler.hpp"

#include <vector>
#include <stdexcept>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <boost/foreach.hpp>

#include "Server.hpp"
#include "Utils/Logger.hpp"
#include "Network/Protocol.hpp"

bool Command::HandleSubcommands(Client* sender, const CommandArgs& args)
{
	for (auto& obj : m_subcommands) {
		if (args[0] == obj->GetName()) {
			CommandArgs subArgs = args;

			subArgs.erase(subArgs.begin());

			// FIXME: Repeated code; have command handler take care of subcommands
			if (subArgs.size() < obj->GetArgumentAmount()) {
				Protocol::SendMessage(sender, "&b" + obj->GetDocString());
				return true;
			}

			obj->Execute(sender, subArgs);

			return true;
		}
	}

	if (!m_subcommands.empty()) {
		Protocol::SendMessage(sender, "&cInvalid subcommand &f" + args.front());
		return true;
	}

	return false;
}

CommandHandler::~CommandHandler()
{
	for (auto& obj : m_commands)
		delete obj.second;
}

void CommandHandler::Reset()
{
	for (auto& obj : m_commands)
		delete obj.second;

	m_commands.clear();
	m_aliases.clear();
}

Command* CommandHandler::GetCommand(std::string name)
{
	auto iter = m_commands.find(name);

	// Not found, check aliases
	if (iter == m_commands.end())
		return nullptr;

	return iter->second;
}

void CommandHandler::Register(std::string name, Command* command, std::string aliases)
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

		size_t size = aliasList.size();
		for (size_t i = 0; i < size; ++i) {
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
			Protocol::SendMessage(sender, "&cYou don't have permission to use that command.");
			return;
		}

		if (args.size() >= iter->second->GetArgumentAmount()) {
			iter->second->Execute(sender, args);
		} else {
			auto helpIter = m_commands.find("help");
			if (helpIter != m_commands.end()) {
				CommandArgs helpArgs = { name };
				helpIter->second->Execute(sender, helpArgs);
			} else {
				Server::SendWrappedMessage(sender, "&b/" + iter->second->GetDocString());
			}
		}
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
