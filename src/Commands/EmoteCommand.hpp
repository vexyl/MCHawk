#ifndef EMOTECOMMAND_H_
#define EMOTECOMMAND_H_

#include <string>

#include "../CommandHandler.hpp"

class EmoteCommand : public ICommand {
public:
	~EmoteCommand() {}

	virtual void Execute(Client* sender, const CommandArgs& args)
	{
		// Quiet compiler
		(void)sender;

		Server* server = Server::GetInstance();

		std::string message;
		for (auto iter = args.begin(); iter != args.end(); ++iter) {
			bool reachedEnd = (iter == (args.end() - 1));
			message += (*iter) + (reachedEnd ? "" : " "); // Leave out pesky space at end
		}

		std::string chatName = sender->GetChatName();

		server->BroadcastMessage("&9* " + chatName + " " + message);
	}

	virtual std::string GetDocString() { return "/me <message> - unleashes an emote upon the world"; }
	virtual unsigned int GetArgumentAmount() { return 1; }
	virtual unsigned int GetPermissionLevel() { return 0; }

private:

};

#endif // EMOTECOMMAND_H_
