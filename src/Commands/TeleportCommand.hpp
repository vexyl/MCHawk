#ifndef TELEPORTCOMMAND_H_
#define TELEPORTCOMMAND_H_

#include <string>

#include "../CommandHandler.hpp"
#include "../Utils/Logger.hpp"

class TeleportCommand : public Command {
public:
	TeleportCommand(std::string name) : Command(name) {}

	~TeleportCommand() {}

	virtual void Execute(Client* sender, const CommandArgs& args) override
	{
		Server* server = Server::GetInstance();
		std::string name = args.front();

		Client* client = server->GetClientByName(name, false);
		if (client == nullptr) {
			Protocol::SendMessage(sender, "&cPlayer &f" + name + "&c does not exist");
			return;
		}

		// Proper name
		name = client->GetName();

		if (name == sender->GetName()) {
			Protocol::SendMessage(sender, "&9*Woosh* &eYou teleport to yourself.");
			return;
		}

		World* srcWorld = sender->GetWorld();
		World* destWorld = client->GetWorld();

		if (srcWorld != destWorld) {
			srcWorld->RemoveClient(sender->GetPid());
			destWorld->AddClient(sender);
		}

		Protocol::SendPosition(sender, -1 /* Self ID */, client->GetPosition(), client->GetYaw(), client->GetPitch());
		Protocol::SendMessage(sender, "&eTeleported to " + name);
	}

	virtual std::string GetDocString() override { return "tp <name> - teleports to player"; }
	virtual unsigned int GetArgumentAmount() override { return 1; }
	virtual unsigned int GetPermissionLevel() override { return 0; }

private:

};

#endif // TELEPORTCOMMAND_H_
