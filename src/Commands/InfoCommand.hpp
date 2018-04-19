#ifndef INFOCOMMAND_H_
#define INFOCOMMAND_H_

#include <string>

#include "../CommandHandler.hpp"
#include "../Network/Protocol.hpp"
#include "../Utils/Logger.hpp"

class InfoCommand : public ICommand {
public:
	~InfoCommand() {}

	virtual void Execute(Client* sender, const CommandArgs& args)
	{
		(void)args; // Quiet compiler
		SendMessage(sender, "&eVexyl's Test Server");
		SendMessage(sender, "&eC++ and SFML-network/boost/zlib");
		SendMessage(sender, "&eDevelopment slow due to real life but still ongoing");
		SendMessage(sender, "&eCurrently supports Linux only");
		SendMessage(sender, "&eThanks for stopping by!");
	}

	virtual std::string GetDocString() { return "/info - displays server info"; }
	virtual unsigned int GetArgumentAmount() { return 0; }
	virtual unsigned int GetPermissionLevel() { return 0; }

private:

};

#endif // INFOCOMMAND_H_
