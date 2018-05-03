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
			ClassicProtocol::SendMessage(sender, "&cPlayer &f" + name + "&c does not exist");
			return;
		}

		ClassicProtocol::SendMessage(sender, "&e--- &f" + client->GetName() + "&e ---");

		std::string worldName = client->GetWorld()->GetName();
		ClassicProtocol::SendMessage(sender, "&eWorld: " + worldName);

		std::string alias = client->GetChatName();
		if (alias != client->GetName())
			ClassicProtocol::SendMessage(sender, "&eAlias: " + alias);
		else
			ClassicProtocol::SendMessage(sender, "&eNo alias");

		if (sender->GetUserType() == 0x64)
			ClassicProtocol::SendMessage(sender, "&eIP: " + client->GetIpString());

	}

	virtual std::string GetDocString() { return "/whois <name> - gets information about player"; }
	virtual unsigned int GetArgumentAmount() { return 0; }
	virtual unsigned int GetPermissionLevel() { return 0; }

private:

};

#endif // WHOISCOMMAND_H_
