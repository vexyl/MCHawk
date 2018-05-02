#ifndef TELEPORTCOMMAND_H_
#define TELEPORTCOMMAND_H_

#include <string>

#include "../CommandHandler.hpp"
#include "../Utils/Logger.hpp"

class TeleportCommand : public ICommand {
public:
	~TeleportCommand() {}

	virtual void Execute(Client* sender, const CommandArgs& args)
	{
		Server* server = Server::GetInstance();
		std::string name = args.front();

		Client* client = server->GetClientByName(name, false);
		if (client == nullptr) {
			SendMessage(sender, "&cPlayer &f" + name + "&c does not exist");
			return;
		}

		// Proper name
		name = client->GetName();

		if (name == sender->GetName()) {
			SendMessage(sender, "&9*Woosh* &eYou teleport to yourself.");
			return;
		}

		std::string srcWorldName = sender->GetWorldName();
		std::string destWorldName = client->GetWorldName();

		if (srcWorldName != destWorldName) {
			World* world = server->GetWorld(destWorldName);
			server->GetWorld(sender->GetWorldName())->RemoveClient(sender->GetPid());
			world->AddClient(sender);
		}

		SendPosition(sender, -1 /* Self ID */, client->GetPosition(), client->GetYaw(), client->GetPitch());
		SendMessage(sender, "&eTeleported to " + name);
	}

	virtual std::string GetDocString() { return "/tp <name> - teleports to player"; }
	virtual unsigned int GetArgumentAmount() { return 1; }
	virtual unsigned int GetPermissionLevel() { return 0; }

private:

};

#endif // TELEPORTCOMMAND_H_
