#ifndef WORLDCOMMAND_H_
#define WORLDCOMMAND_H_

#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "../CommandHandler.hpp"
#include "../Utils/Logger.hpp"

class WorldCommand : public ICommand {
public:
	~WorldCommand() {}

	virtual void Execute(Client* sender, const CommandArgs& args)
	{
		Server* server = Server::GetInstance();
		std::string subcommand = args.front();
		bool isOperator = sender->GetUserType() >= 0x64;

		boost::algorithm::to_lower(subcommand);

		if (subcommand == "list") {
			std::vector<std::string> worldNames = server->GetWorldNames();

			SendMessage(sender, "&eList of worlds:");
			for (auto& name : worldNames)
				SendMessage(sender, "&e- " + name);
		} else if (subcommand == "new") {
			if (!isOperator) {
				SendMessage(sender, "&cOnly operators can create new worlds at this time");
				return;
			}

			if (args.size() < 5) {
				SendMessage(sender, "&c/world new <name> <x> <y> <z>");
				return;
			}

			std::string worldName = args[1];
			boost::algorithm::to_lower(worldName);

			if (worldName.length() > 10) {
				SendMessage(sender, "&cInvalid name");
				return;
			}

			short x, y, z;
			try {
				x = boost::lexical_cast<short>(args[2]);
				y = boost::lexical_cast<short>(args[3]);
				z = boost::lexical_cast<short>(args[4]);
			} catch (boost::bad_lexical_cast&) {
				SendMessage(sender, "&cMalformed input");
				return;
			}

			if (x <= 0 || x > 1024 || y <= 0 || y > 64 || z <= 0 || z > 1024) {
				SendMessage(sender, "&cInvalid size");
				return;
			}

			//FIXME TEMPORARY
			World* w = new World(worldName);
			w->GetMap().GenerateFlatMap(x, y, z);
			w->SetSpawnPosition(Position(x/2*32+51, y/2*32+51, z/2*32+51));
			w->SetOption("autosave", "false");

			server->AddWorld(w);

			SendMessage(sender, "&eWorld " + worldName + " created (" + std::to_string(x) + "x" + std::to_string(y) + "x" + std::to_string(z) + ")");
		} else if (subcommand == "set") {
			if (!isOperator) {
				SendMessage(sender, "&cOnly operators can set world options");
				return;
			}

			if (args.size() < 3) {
				SendMessage(sender, "&c/world set <option> <value>");
				return;
			}

			std::string option = args[1];
			std::string value = args[2];

			// FIXME: Temporary
			if (option != "autosave" && option != "build") {
				SendMessage(sender, "&cInvalid option");
				return;
			}

			// FIXME: Temporary
			if (value != "true" && value != "false") {
				SendMessage(sender, "&cInvalid value");
				return;
			}

			World *w = server->GetWorld(sender->GetWorldName());
			w->SetOption(option, value);

			SendMessage(sender, "&eWorld option " + option + "=" + value);
		} else {
			SendMessage(sender, "&b" + GetDocString());
			return;
		}
	}

	virtual std::string GetDocString() { return "/world <list/new*/set*> - various world-related commands | * operator only"; }
	virtual unsigned int GetArgumentAmount() { return 1; }
	virtual unsigned int GetPermissionLevel() { return 0; }

private:

};

#endif // WORLDCOMMAND_H_
