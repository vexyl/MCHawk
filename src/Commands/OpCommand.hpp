#ifndef OPCOMMAND_H_
#define OPCOMMAND_H_

#include <string>

#include "../CommandHandler.hpp"
#include "../Network/Protocol.hpp"
#include "../Utils/Logger.hpp"

class OpCommand : public ICommand {
public:
	~OpCommand() {}

	virtual void Execute(Client* sender, const CommandArgs& args)
	{
		Server* server = Server::GetInstance();
		std::string name = args.front();

		Client* client = server->GetClientByName(name, false);
		if (client == nullptr) {
			SendMessage(sender, "&cPlayer &f" + name + "&c does not exist");
			return;
		}

		ToggleOp(sender, client);

	}

	void ToggleOp(Client* sender, Client* client)
	{
		Server* server = Server::GetInstance();
		uint8_t userType = client->GetUserType();
		std::string name = client->GetName();

		uint8_t newUserType = (userType == 0x00 ? 0x64 : 0x00);
		if (newUserType == 0x00) {
			server->SendSystemMessage(client, "You've been deopped.");
			server->SendSystemMessage(sender, "Deopped " + name);
		} else {
			server->SendSystemMessage(client, "You've been opped.");
			server->SendSystemMessage(sender, "Opped " + name);
		}

		client->SetUserType(newUserType);
		SendUserType(client, newUserType);
	}

	virtual std::string GetDocString() { return "/op <name> - toggles operator status on a player"; }
	virtual unsigned int GetArgumentAmount() { return 1; }
	virtual unsigned int GetPermissionLevel() { return 1; }

private:

};

#endif // OPCOMMAND_H_
