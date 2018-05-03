#ifndef SAVECOMMAND_H_
#define SAVECOMMAND_H_

#include <string>

#include "../CommandHandler.hpp"
#include "../Network/Protocol.hpp"
#include "../Utils/Logger.hpp"

class SaveCommand : public ICommand {
public:
	~SaveCommand() {}

	virtual void Execute(Client* sender, const CommandArgs& args)
	{
		(void)args; // Quiet compiler

		bool saved = sender->GetWorld()->SaveMapIfChanged();
		if (saved)
			ClassicProtocol::SendMessage(sender, "&eMap saved to file");
		else
			ClassicProtocol::SendMessage(sender, "&cNo changes to save");
	}

	virtual std::string GetDocString() { return "/save - saves current map to file"; }
	virtual unsigned int GetArgumentAmount() { return 0; }
	virtual unsigned int GetPermissionLevel() { return 1; }

private:

};

#endif // SAVECOMMAND_H_
