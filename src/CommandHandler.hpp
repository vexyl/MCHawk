#ifndef COMMANDHANDLER_H_
#define COMMANDHANDLER_H_

#include <string>
#include <vector>
#include <map>

#include "Client.hpp"

typedef std::vector<std::string> CommandArgs;

class ICommand {
public:
	virtual ~ICommand() {}
	virtual void Execute(Client* sender, const CommandArgs& args) = 0;
	virtual std::string GetDocString() = 0;
	virtual unsigned int GetArgumentAmount() = 0;
	virtual unsigned int GetPermissionLevel() = 0;
};

typedef std::map<std::string, ICommand*> CommandMap;
typedef std::map<std::string, std::string> AliasMap;

class CommandHandler {
public:
	CommandHandler() {}

	virtual ~CommandHandler();

	typedef std::map<std::string, ICommand*> CommandMap;

	const CommandMap& GetCommandList() const { return m_commands; }

	void Register(std::string name, ICommand* command, std::string aliases="");
	void Execute(Client* sender, std::string name, const CommandArgs& args);
	void Handle(Client* sender, std::string message);

private:
	std::map<std::string, ICommand*> m_commands;
	std::map<std::string, std::string> m_aliases;
};

#endif // COMMANDHANDLER_H_
