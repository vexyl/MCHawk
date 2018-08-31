#ifndef SUMMONCOMMAND_H_
#define SUMMONCOMMAND_H_

#include <string>

#include "../CommandHandler.hpp"
#include "../Network/Protocol.hpp"
#include "../Utils/Logger.hpp"


class SummonCommand : public Command {
public:
	SummonCommand(std::string name) : Command(name) {}

	~SummonCommand() {}

	virtual void Execute(Client* sender, const CommandArgs& args)
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

		std::string senderName = sender->GetName();
		if (name == senderName) {
			Protocol::SendMessage(sender, "&eSure, if it makes you feel better... &9*Woosh*");
			return;
		}

		World* srcWorld = sender->GetWorld();
		World* destWorld = client->GetWorld();

		if (srcWorld != destWorld) {
			srcWorld->RemoveClient(sender->GetPid());
			destWorld->AddClient(sender);
		}

		Protocol::SendPosition(client, -1 /* Self ID */, sender->GetPosition(), client->GetYaw(), client->GetPitch());
		Protocol::SendMessage(sender, "&eSummoned player " + name);
		Protocol::SendMessage(client, "&e" + senderName + " has summoned you");
	}

	virtual std::string GetDocString() { return "summon <name> - summons a player to your position"; }
	virtual unsigned int GetArgumentAmount() { return 1; }
	virtual unsigned int GetPermissionLevel() { return 1; }

private:

};

#endif // SUMMONCOMMAND_H_
