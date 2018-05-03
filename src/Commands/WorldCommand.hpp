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

			int size = (int)worldNames.size();
			std::string message = "&eWorlds: ";

			for (int i = 0; i < size; ++i) {
				if (server->GetWorld(worldNames[i])->GetActive())
					message += "&a";
				else
					message += "&c";

				message += worldNames[i];

				if (i < size - 1)
					message += "&e, ";
			}

			Server::SendWrappedMessage(sender, message);
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
			std::string filename = "worlds/maps/" + worldName + "_" + std::to_string(x) + "x" + std::to_string(y) + "x" + std::to_string(z) + ".raw";

			World* world = new World(worldName);

			world->GetMap().GenerateFlatMap(filename, x, y, z);
			world->GetMap().SetFilename(filename);
			world->SetSpawnPosition(Position(x/2*32+51, y/2*32+51, z/2*32+51));
			world->SetOption("autosave", "true");
			world->SetActive(true);

			server->AddWorld(world);

			SendMessage(sender, "&eWorld &a" + worldName + "&e created (" + std::to_string(x) + "x" + std::to_string(y) + "x" + std::to_string(z) + ")");
		} else if (subcommand == "set") {
			if (!isOperator) {
				SendMessage(sender, "&cOnly operators can set world options");
				return;
			}

			World *w = sender->GetWorld();

			if (args.size() < 2) {
				std::vector<std::string> options = w->GetOptionNames();
				std::string message = "&eWorld options: ";

				int size = (int)options.size();

				if (size == 0) {
					SendMessage(sender, "No options");
					return;
				}

				for (int i = 0; i < size; ++i) {
					message += options[i];
					if (i < size - 1)
						message += ", ";
				}

				Server::SendWrappedMessage(sender, message);

				return;
			}

			std::string option = args[1];

			bool validOption = w->IsValidOption(option);
			if (!validOption) {
				SendMessage(sender, "&cInvalid option");
				return;
			}

			if (args.size() == 2) {
				std::string value = w->GetOption(option);
				SendMessage(sender, "&e" + option + "=" + value);
				return;
			}

			std::string value = args[2];

			// FIXME: Temporary
			if (value != "true" && value != "false") {
				SendMessage(sender, "&cInvalid value");
				return;
			}

			w->SetOption(option, value);

			SendMessage(sender, "&eWorld option " + option + "=" + value);
		} else if (subcommand == "load") {
			if (args.size() < 2) {
				SendMessage(sender, "&cMust specify world name");
				return;
			}

			std::string worldName = args[1];

			boost::algorithm::to_lower(worldName);

			World *world = server->GetWorld(worldName);

			if (world == nullptr) {
				SendMessage(sender, "&cWorld &f" + worldName + "&c does not exist");
				return;
			}

			if (world->GetActive()) {
				SendMessage(sender, "&cWorld &a" + worldName + "&c is already loaded");
				return;
			}

			world->GetMap().Load();
			world->SetActive(true);

			SendMessage(sender, "&eLoaded world &a" + worldName);
		} else if (subcommand == "save") {
			if (!isOperator) {
				SendMessage(sender, "&cOnly operators can save worlds");
				return;
			}

			if (args.size() < 2) {
				SendMessage(sender, "&cMust specify world name");
				return;
			}

			std::string worldName = args[1];

			boost::algorithm::to_lower(worldName);

			World *world = server->GetWorld(worldName);

			if (world == nullptr) {
				SendMessage(sender, "&cWorld &f" + worldName + "&c does not exist");
				return;
			}

			world->Save();

			SendMessage(sender, "&eSaved world &a" + worldName);
		} else if (subcommand == "spawn") {
			// FIXME: Temporary
			Position pos = sender->GetPosition();
			World *w = sender->GetWorld();

			w->SetSpawnPosition(pos);

			SendMessage(sender, "&eSpawn position set (" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z) + ")");
		} else {
			SendMessage(sender, "&b" + GetDocString());
			return;
		}
	}

	virtual std::string GetDocString() { return "/world <list/load/new*/save*/set*> - various world-related commands | * operator only"; }
	virtual unsigned int GetArgumentAmount() { return 1; }
	virtual unsigned int GetPermissionLevel() { return 0; }

private:

};

#endif // WORLDCOMMAND_H_
