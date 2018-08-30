/*
 * Vexyl
 * Started in March, 2017
 */

#include "Server.hpp"

#include <iostream>

#include <cstdlib>
#include <ctime>

#ifdef _WIN32
	#pragma comment(lib, "Ws2_32.lib")
#endif

int main()
{
	std::srand(std::time(0));

	sf::Clock clock;
	Server* server = Server::GetInstance();

	server->Init();

	bool running = true;
	while (running) {
		clock.restart();

		running = server->Tick();

		// Sleep up to 33ms
		sf::Time time = clock.getElapsedTime();
		int ms = time.asMilliseconds();
		if (ms < 33) {
			sf::sleep(sf::milliseconds(33 - ms));
		}
	}

	server->FreeInstance();

	std::cout << "Exiting normally." << std::endl;

	return 0;
}
