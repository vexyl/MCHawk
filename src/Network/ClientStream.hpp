#ifndef CLIENTSTREAM_H_
#define CLIENTSTREAM_H_

#include <string.h>

#include <SFML/Network.hpp>

// Copied from moderator hplus0603 at gamedev.net
// Modified
struct ClientStream {
	sf::TcpSocket *socket;
	size_t count;
	char buf[8192];

	ClientStream() {
		socket = nullptr;
		count = 0;
	}

	sf::Socket::Status poll() {
		if (count == 8192) { return sf::Socket::Status::NotReady; }
			size_t size = 8192-count;

			size_t r = 0;
			sf::Socket::Status status = socket->receive((void*)&buf[count], size, r);

			if (r > 0) { count += r; }

			return status;
	}

	bool consume_data(void *dst, size_t size) {
		if (count < size) { return false; }
		memcpy(dst, buf, size);
		memmove(buf, &buf[size], count-size);
		count -= size;
		return true;
	}
};

#endif // CLIENTSTREAM_H_
