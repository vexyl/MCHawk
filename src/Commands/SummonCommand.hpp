#ifndef SUMMONCOMMAND_H_
#define SUMMONCOMMAND_H_

#include <string>

#include "../CommandHandler.hpp"
#include "../Network/Protocol.hpp"
#include "../Utils/Logger.hpp"


class SummonCommand : public ICommand {
public:
	~SummonCommand() {}

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

		std::string senderName = sender->GetName();
		if (name == senderName) {
			SendMessage(sender, "&eSure, if it makes you feel better... &9*Woosh*");
			return;
		}

		std::string srcWorldName = client->GetWorld()->GetName();
		std::string destWorldName = sender->GetWorld()->GetName();

		if (srcWorldName != destWorldName) {
			World* world = server->GetWorld(destWorldName);
			client->GetWorld()->RemoveClient(client->GetPid());
			world->AddClient(client);
		}

		SendPosition(client, -1 /* Self ID */, sender->GetPosition(), client->GetYaw(), client->GetPitch());
		SendMessage(sender, "&eSummoned player " + name);
		SendMessage(client, "&e" + senderName + " has summoned you");
	}

	virtual std::string GetDocString() { return "/summon <name> - summons a player to your position"; }
	virtual unsigned int GetArgumentAmount() { return 1; }
	virtual unsigned int GetPermissionLevel() { return 1; }

private:

};

#endif // SUMMONCOMMAND_H_
