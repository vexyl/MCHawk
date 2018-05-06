#ifndef ALIASCOMMAND_H_
#define ALIASCOMMAND_H_

#include <string>

#include "../CommandHandler.hpp"
#include "../Network/Protocol.hpp"
#include "../Utils/Logger.hpp"


class AliasCommand : public ICommand {
public:
	~AliasCommand() {}

	virtual void Execute(Client* sender, const CommandArgs& args)
	{
		std::string name;

		// Default chat name to username
		if (args.empty()) {
			name = sender->GetName();

			sender->SetChatName(name);
			Server::SendWrappedMessage(sender, "&eAlias defaulted to " + name);

			return;
		}

		name = args.front();

		char validChars[64] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_";
		size_t length = name.length();

		// Alphanumeric names only, max length 16, color codes are ok
		if (length == 0 || length > 16 || name.find_first_not_of(validChars) != std::string::npos) {
			Protocol::SendMessage(sender, "&cInvalid name");
			return;
		}

		sender->SetChatName(name);
		Server::SendWrappedMessage(sender, "&eAlias set to " + name);
	}

	virtual std::string GetDocString() { return "/alias [name] - change chat name; defaults to username"; }
	virtual unsigned int GetArgumentAmount() { return 0; }
	virtual unsigned int GetPermissionLevel() { return 1; }

private:

};

#endif // ALIASCOMMAND_H_
