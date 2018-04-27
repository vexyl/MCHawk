#ifndef GOTOCOMMAND_H_
#define GOTOCOMMAND_H_

#include <string>
#include <boost/algorithm/string.hpp>

#include "../CommandHandler.hpp"
#include "../Network/Protocol.hpp"
#include "../Utils/Logger.hpp"

class GotoCommand : public ICommand {
public:
	~GotoCommand() {}

	virtual void Execute(Client* sender, const CommandArgs& args)
	{
		Server* server = Server::GetInstance();
		std::string worldName = args.front();

		boost::algorithm::to_lower(worldName);

		World* world = server->GetWorld(worldName);
		if (world == nullptr) {
			SendMessage(sender, "&cWorld " + worldName + " does not exist");
			return;
		}

		if (!world->GetActive()) {
			SendMessage(sender, "&cWorld " + worldName + " is not loaded");
			return;
		}

		if (worldName == sender->GetWorldName()) {
			SendMessage(sender, "&eWarp nine. Engage. &9*Woosh*");
			return;
		}

		server->GetWorld(sender->GetWorldName())->RemoveClient(sender->GetPid());
		world->AddClient(sender);
		SendMessage(sender, "&eWarped to " + worldName);
	}

	virtual std::string GetDocString() { return "/goto <world name> - warps to world"; }
	virtual unsigned int GetArgumentAmount() { return 1; }
	virtual unsigned int GetPermissionLevel() { return 0; }

private:

};

#endif // GOTOCOMMAND_H_
