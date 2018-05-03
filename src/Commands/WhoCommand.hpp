#ifndef WHOCOMMAND_H_
#define WHOCOMMAND_H_

#include <string>

#include "../CommandHandler.hpp"
#include "../Utils/Logger.hpp"

class WhoCommand : public ICommand {
public:
	~WhoCommand() {}

	virtual void Execute(Client* sender, const CommandArgs& args)
	{
		(void)args; // Quiet compiler

		Server* server = Server::GetInstance();
		std::vector<Client*> clients = server->GetClients();

		std::string message = "&ePlayer list: ";
		int size = (int)clients.size();
		for (int i = 0; i < size; ++i) {
			message += "&6" + clients[i]->GetName() + "&e(&a" + clients[i]->GetWorld()->GetName() + "&e)";
			if (i < size - 1)
				message += ", ";
		}

		Server::SendWrappedMessage(sender, message);

	}

	virtual std::string GetDocString() { return "/who - gets player list"; }
	virtual unsigned int GetArgumentAmount() { return 0; }
	virtual unsigned int GetPermissionLevel() { return 0; }

private:

};

#endif // WHOCOMMAND_H_
