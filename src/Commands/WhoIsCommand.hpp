#ifndef WHOISCOMMAND_H_
#define WHOISCOMMAND_H_

#include <string>

#include "../CommandHandler.hpp"
#include "../Utils/Logger.hpp"

class WhoIsCommand : public ICommand {
public:
	~WhoIsCommand() {}

	virtual void Execute(Client* sender, const CommandArgs& args)
	{
		Server* server = Server::GetInstance();
		std::string name;

		if (args.empty())
			name = sender->GetName();
		else
			name = args.front();

		Client* client = server->GetClientByName(name, false);
		if (client == nullptr) {
			SendMessage(sender, "&cPlayer " + name + " does not exist");
			return;
		}

		SendMessage(sender, "&e--- &f" + client->GetName() + "&e ---");

		std::string worldName = client->GetWorldName();
		SendMessage(sender, "&eWorld: " + worldName);

		std::string alias = client->GetChatName();
		if (alias != client->GetName())
			SendMessage(sender, "&eAlias: " + alias);
		else
			SendMessage(sender, "&eNo alias");

		if (sender->GetUserType() == 0x64)
			SendMessage(sender, "&eIP: " + client->GetIpString());

	}

	virtual std::string GetDocString() { return "/whois <name> - gets information about player"; }
	virtual unsigned int GetArgumentAmount() { return 0; }
	virtual unsigned int GetPermissionLevel() { return 0; }

private:

};

#endif // WHOISCOMMAND_H_
