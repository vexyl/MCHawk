#ifndef BILLNYECOMMAND_H_
#define BILLNYECOMMAND_H_

#include <string>

#include "../CommandHandler.hpp"

class BillNyeCommand : public ICommand {
public:
	~BillNyeCommand() {}

	virtual void Execute(Client* sender, const CommandArgs& args)
	{
		// Quiet compiler
		(void)sender;

		Server* server = Server::GetInstance();

		std::string wisdom;
		for (auto iter = args.begin(); iter != args.end(); ++iter) {
			bool reachedEnd = (iter == (args.end() - 1));
			wisdom += (*iter) + (reachedEnd ? "" : " "); // Leave out pesky space at end
		}

		server->BroadcastMessage("&cBill Nye&f: DID YOU KNOW THAT");
		server->BroadcastMessage("&cBill Nye&f: " + wisdom + "?");
		server->BroadcastMessage("&cBill Nye&f: NOW YOU KNOW!");
	}

	virtual std::string GetDocString() { return "/billnye <wisdom> - instills wisdom in fellow server members"; }
	virtual unsigned int GetArgumentAmount() { return 1; }
	virtual unsigned int GetPermissionLevel() { return 0; }

private:

};

#endif // BILLNYECOMMAND_H_
