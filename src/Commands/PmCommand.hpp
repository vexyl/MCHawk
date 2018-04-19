#ifndef PMCOMMAND_H_
#define PMCOMMAND_H_

#include <string>

#include "../CommandHandler.hpp"
#include "../Network/Protocol.hpp"
#include "../Utils/Logger.hpp"

class PmCommand : public ICommand {
public:
	~PmCommand() {}

	virtual void Execute(Client* sender, const CommandArgs& args)
	{
		Server* server = Server::GetInstance();
		std::string name = args.front();

		Client* client = server->GetClientByName(name, false);
		if (client == nullptr) {
			SendMessage(sender, "&cPlayer " + name + " does not exist");
			return;
		}

		// Proper name
		name = client->GetName();

		std::string senderName = sender->GetName();
		if (name == senderName) {
			SendMessage(sender, "&cYou shouldn't be talking to yourself...");
			return;
		}

		std::string message;
		for (auto iter = args.begin()+1; iter != args.end(); ++iter) {
			bool reachedEnd = (iter == (args.end() - 1));
			message += (*iter) + (reachedEnd ? "" : " "); // Leave out pesky space at end
		}

		SendMessage(sender, "&d[pm->" + name + "] " + message);
		SendMessage(client, "&d[" + senderName + "] " + message);
	}

	virtual std::string GetDocString() { return "/pm <name> <message> - private message a player"; }
	virtual unsigned int GetArgumentAmount() { return 2; }
	virtual unsigned int GetPermissionLevel() { return 0; }

private:

};

#endif // PMCOMMAND_H_
