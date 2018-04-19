#ifndef HELPCOMMAND_H_
#define HELPCOMMAND_H_

#include <cmath>

#include <string>

#include "../CommandHandler.hpp"
#include "../Network/Protocol.hpp"
#include "../Utils/Logger.hpp"

class HelpCommand : public ICommand {
public:
	~HelpCommand() {}

	virtual void Execute(Client* sender, const CommandArgs& args)
	{
		(void)args; // Quiet compiler

		Server* server = Server::GetInstance();
		uint8_t userType = sender->GetUserType();

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
		if (!args.empty()) {
			std::string commandName = args.front();

			// Does this command exist?
			auto iter = commands.find(commandName);
			if (iter != commands.end()) {
				// issued /help commandName
				SendMessage(sender, "&b" + iter->second->GetDocString());
				return;
			}
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
				SendMessage(sender, "&cInvalid argument");
				return;
			} else {
				page = atoi(pageString.c_str());
				if (page <= 0)
					page = 1;

				if (page > maxPages) {
					SendMessage(sender, "&cInvalid page number");
					return;
				}
			}
		}

		SendMessage(sender, "&f--- &bCommands &f---");

		offset = (page - 1) * maxResults;
		for (unsigned int i = 0; i < maxResults && (offset+i) < commands.size(); ++i) {
			auto it = commands.begin();

			std::advance(it, offset+i);

			SendMessage(sender, "&b" + it->second->GetDocString());
		}

		SendMessage(sender, "&f--- &bpage " + std::to_string(page) + " of " + std::to_string(maxPages) + " &f---");
	}

	virtual std::string GetDocString() { return "/help [command/page number] - displays this help message"; }
	virtual unsigned int GetArgumentAmount() { return 0; }
	virtual unsigned int GetPermissionLevel() { return 0; }

private:

};

#endif // HELPCOMMAND_H_
