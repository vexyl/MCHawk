#ifndef HELPCOMMAND_H_
#define HELPCOMMAND_H_

#include <cmath>

#include <string>

#include "../CommandHandler.hpp"
#include "../Network/Protocol.hpp"
#include "../Utils/Logger.hpp"

class HelpCommand : public Command {
public:
	HelpCommand(std::string name) : Command(name) {}

	~HelpCommand() {}

	virtual void Execute(Client* sender, const CommandArgs& args)
	{
		(void)args; // Quiet compiler

		Server* server = Server::GetInstance();
		uint8_t userType = sender->GetUserType();

		if (args.empty()) {
			Protocol::SendMessage(sender, "&eTo see a list of commands type &a/commands");
			Protocol::SendMessage(sender, "&eTo get help with a specific command, type &a/help <command>");
			Protocol::SendMessage(sender, "&eFor pages of commands, type &a/help <page #>");
			Protocol::SendMessage(sender, "&eFor a list of worlds, type &a/world list");
			Protocol::SendMessage(sender, "&eTo go to a world, type &a/goto <world>");
			Protocol::SendMessage(sender, "&eTo see a list of players, type &a/who &eor &a/players");
			Protocol::SendMessage(sender, "&eFor information about the server, type &a/serverinfo");
			return;
		}

		auto commands = server->GetCommandHandler().GetCommandList();

		// Remove commands sender can't execute
		auto it = commands.begin();
		while (it != commands.end()) {
			bool hasPermission = (userType >= it->second->GetPermissionLevel());
			if (!hasPermission)
				it = commands.erase(it);
			else
				++it;
		}

		// Check if client wants command docstring
		std::string commandName = args.front();

		// Does this command exist?
		auto iter = commands.find(commandName);
		if (iter != commands.end()) {
			// issued /help commandName
			Command* subcheck = iter->second;
			std::string docStringPrefix = "/";

			if (args.size() > 1) {
				int i = 1;
				while (i < (int)args.size()) {
					auto subcommands = subcheck->GetSubcommands();

					for (auto& obj : subcommands) {
						if (args[i] == obj->GetName()) {
							docStringPrefix += subcheck->GetName() + " "; // previous command/subcommand
							subcheck = obj;
							break;
						}
					}

					++i;
				}
			}

			Server::SendWrappedMessage(sender, "&b" + docStringPrefix + subcheck->GetDocString());

			auto subcommands = subcheck->GetSubcommands();

			std::string subcommandString;
			for (int i = 0; i < (int)subcommands.size(); ++i) {
				subcommandString += "&a" + subcommands[i]->GetName();
				if ((i + 1) < (int)subcommands.size())
					subcommandString += "&b, ";
			}

			if (!subcommandString.empty())
				Server::SendWrappedMessage(sender, "&bAvailable subcommands: " + subcommandString);

			return;
		}

		// From here any remaining args treated as page number
		unsigned int page = 1;
		unsigned int offset = 0;
		unsigned int maxResults = 10;
		unsigned int maxPages = (unsigned int)(std::ceil((float)commands.size() / (float)maxResults));

		std::string pageString;
		if (args.size() > 0) {
			pageString = args.front();
			// Is a valid number?
			if (pageString.find_first_not_of("1234567890") != std::string::npos) {
				Protocol::SendMessage(sender, "&cInvalid argument");
				return;
			} else {
				page = atoi(pageString.c_str());
				if (page <= 0)
					page = 1;

				if (page > maxPages) {
					Protocol::SendMessage(sender, "&cInvalid page number");
					return;
				}
			}
		}

		Protocol::SendMessage(sender, "&f--- &bCommands &f---");

		offset = (page - 1) * maxResults;
		for (unsigned int i = 0; i < maxResults && (offset+i) < commands.size(); ++i) {
			auto it = commands.begin();

			std::advance(it, offset+i);

			Server::SendWrappedMessage(sender, "&b/" + it->second->GetDocString());
		}

		Protocol::SendMessage(sender, "&f--- &bpage " + std::to_string(page) + " of " + std::to_string(maxPages) + " &f---");
	}

	virtual std::string GetDocString() { return "help [command/page number] - displays this help message"; }
	virtual unsigned int GetArgumentAmount() { return 0; }
	virtual unsigned int GetPermissionLevel() { return 0; }

private:

};

#endif // HELPCOMMAND_H_
