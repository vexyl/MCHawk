#include "Server.hpp"

int main()
{
	srand(time(0));

	sf::Clock clock;
	Server* server = Server::GetInstance();

	server->Init();

	bool running = true;
	while (running) {
		clock.restart();

		server->Tick();

		// Sleep up to 33ms
		sf::Time time = clock.getElapsedTime();
		int ms = time.asMilliseconds();
		if (ms < 33) {
			sf::sleep(sf::milliseconds(33 - ms));
		}
	}

	return 0;
}
