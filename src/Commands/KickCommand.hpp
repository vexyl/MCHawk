#ifndef KICKCOMMAND_H_
#define KICKCOMMAND_H_

#include <string>

#include "../CommandHandler.hpp"
#include "../Network/Protocol.hpp"
#include "../Utils/Logger.hpp"

class KickCommand : public ICommand {
public:
	~KickCommand() {}

	virtual void Execute(Client* sender, const CommandArgs& args)
	{
		Server* server = Server::GetInstance();

		std::string name = args.front();

		Client* client = server->GetClientByName(name, false);
		if (client == nullptr) {
			ClassicProtocol::SendMessage(sender, "&cPlayer &f" + name + "&c does not exist");
			return;
		}

		// Proper name
		name = client->GetName();

		std::string reason;
		if (args.size() > 1) {
			for (auto iter = args.begin() + 1; iter != args.end(); ++iter) {
				bool reachedEnd = (iter == (args.end() - 1));
				reason += (*iter) + (reachedEnd ? "" : " "); // Leave out pesky space at end
			}
		}

		server->KickClient(client, reason);
		if (reason.empty())
			server->SendSystemWideMessage("Player " + name + " kicked");
		else
			server->SendSystemWideMessage("Player " + name + " kicked (" + reason + ")");
	}

	virtual std::string GetDocString() { return "/kick <name> [reason] - kicks player from server"; }
	virtual unsigned int GetArgumentAmount() { return 1; }
	virtual unsigned int GetPermissionLevel() { return 1; }

private:

};

#endif // KICKCOMMAND_H_
