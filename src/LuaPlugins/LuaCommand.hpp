#ifndef LUACOMMAND_H_
#define LUACOMMAND_H_

#include <string>
#include <memory>

#include "../CommandHandler.hpp"
#include "../Utils/Logger.hpp"
#include "LuaStuff.hpp"

luabridge::LuaRef make_luatable();

class LuaCommand : public Command {
public:
  LuaCommand(std::string name, luabridge::LuaRef func, std::string docString,
      unsigned argumentAmount, unsigned permissionLevel) :
    Command(name),
    m_docString(docString), m_argumentAmount(argumentAmount), m_permissionLevel(permissionLevel)
  {
    if (!func.isFunction()) {
      LOG(LogLevel::kWarning, "Failed to load Lua command %s", name.c_str());
      return;
    }

    m_func = std::make_unique<luabridge::LuaRef>(func);
  }

  ~LuaCommand() {}

  virtual void Execute(Client* sender, const CommandArgs& args) override
  {
    if (!args.empty() && !m_subcommands.empty()
        && HandleSubcommands(sender, args) != nullptr)
      return;

    if (m_func != nullptr) {
      auto table = make_luatable();

      for (int i = 1; i < (int)args.size() + 1; ++i)
        table[i] = args[i-1];

      (*m_func)(sender, table);
    }
  }

  virtual std::string GetDocString() override { return m_docString; }
  virtual unsigned int GetArgumentAmount() override { return m_argumentAmount; }
  virtual unsigned int GetPermissionLevel() override { return m_permissionLevel; }

  LuaCommand* AddSubcommand(std::string subcommand, luabridge::LuaRef func, std::string docString,
    unsigned argumentAmount, unsigned permissionLevel)
  {
    if (!func.isFunction())
      throw std::runtime_error("Failed adding Lua subcommand " + subcommand + ": function does not exist");

    LuaCommand* command = new LuaCommand(subcommand, func, docString, argumentAmount, permissionLevel);

    Command::AddSubcommand(command);

    return command;
  }

private:
  std::unique_ptr<luabridge::LuaRef> m_func;
  std::string m_docString;
  unsigned int m_argumentAmount, m_permissionLevel;
};

#endif // LUACOMMAND_H_
