# MCHawk

This is a Minecraft classic server written in C++.

Libraries: SFML, Boost, LuaBridge, OpenSSL, and zlib

## Compile

When cloning this repository make sure the submodule LuaBridge is also cloned

```
git clone --recursive https://github.com/vexyl/MCHawk.git
```

On Linux, install the above libraries with your distribution's package manager

### Linux

**Arch Linux:**

```
pacman -S gcc make sfml boost openssl zlib lua52
```

**Debian:**

```
apt-get install g++ make libsfml-dev libboost-all-dev libssl-dev libz-dev liblua5.2-dev
```

and then `cd` into the source directory and run

```
make
```

the binary will output to *bin/Release*.

### Windows

First, download the dependencies from [Dropbox](https://www.dropbox.com/s/t5lg7s7wom0ybft/MCHawk_dependencies.zip?dl=0).

Extract MCHawk_dependencies into the MCHawk win32 folder. It should look like *MCHawk/win32/dependencies*.

Open the VS2017 project *MCHawk/win32/MCHawk.sln* and set the configuration to Release x86.

Build the project and copy contents from *MCHawk/win32/Release* to *MCHawk/bin/Release*.

The same applies to Debug with the appropriate filename changes to the above instructions.

## Setup

Copy the plugins folder to *bin/Release/* and run *bin/Release/MCHawk* to start the server.

## Misc

The release version sends a heartbeat request to http://www.classicube.net by default. Edit *bin/Release/config.ini* to change it.

Discord: https://discord.gg/xwDejUa
