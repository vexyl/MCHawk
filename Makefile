SRC = \
	./src/Main.cpp \
	./src/Server.cpp \
	./src/Client.cpp \
	./src/World.cpp \
	./src/Map.cpp \
	./src/CommandHandler.cpp \
	./src/Network/Packet.cpp \
	./src/Network/Protocol.cpp \
	./src/Utils/BufferStream.cpp \
	./src/Utils/Logger.cpp \
	./src/Utils/Utils.cpp
HEADERS = \
	./src/Server.hpp \
	./src/Client.hpp \
	./src/World.hpp \
	./src/Map.hpp \
	./src/Position.hpp \
	./src/CommandHandler.hpp \
	./src/Network/ClientStream.hpp \
	./src/Network/Packet.hpp \
	./src/Network/Protocol.hpp \
	./src/Utils/BufferStream.hpp \
	./src/Utils/Logger.hpp \
	./src/Utils/Utils.hpp \
	./src/Commands/*.hpp

TARGET = MCHawk
LIBS = -lsfml-system -lsfml-network -lz -lboost_system -lcrypto
OPTS = -Wall -Wextra -pedantic-errors -Wfatal-errors -std=c++11
FLAGS = $(LIBS) $(OPTS)
OUT = ./bin/Release/$(TARGET)
DEBUG = ./bin/Debug/$(TARGET)
CC = g++

default: $(OUT)

debug: $(DEBUG)

$(OUT): $(SRC) $(HEADERS)
	$(CC) -o $(OUT) $(SRC) $(FLAGS)

$(DEBUG): $(SRC) $(HEADERS)
	$(CC) -o $(DEBUG) $(SRC) $(FLAGS)

clean:
	rm $(OUT) -f
	rm $(DEBUG) -f
