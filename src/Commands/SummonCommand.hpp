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

		std::string senderName = sender->GetName();
		if (name == senderName) {
			Protocol::SendMessage(sender, "&eSure, if it makes you feel better... &9*Woosh*");
			return;
		}

		World* srcWorld = client->GetWorld();
		World* destWorld = sender->GetWorld();

		if (srcWorld != destWorld) {
			srcWorld->RemoveClient(client->GetPid());
			destWorld->AddClient(client);
		}

		Protocol::SendPosition(client, -1 /* Self ID */, sender->GetPosition(), client->GetYaw(), client->GetPitch());

		Protocol::SendMessage(sender, "&eSummoned player " + name);
		Protocol::SendMessage(client, "&e" + senderName + " has summoned you");
	}

	virtual std::string GetDocString() override { return "summon <name> - summons a player to your position"; }
	virtual unsigned int GetArgumentAmount() override { return 1; }
	virtual unsigned int GetPermissionLevel() override { return 1; }

private:

};

#endif // SUMMONCOMMAND_H_
