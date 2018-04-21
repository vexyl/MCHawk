# MCHawk

This is a Minecraft classic server written in C++.

Libraries: SFML, Boost, OpenSSL, and zlib

## Compile

On Linux, install the above libraries with your distribution's package manager

**Arch Linux:**

```
pacman -S gcc make sfml boost openssl zlib
```

**Debian:**

```
apt-get install g++ make libsfml-dev libboost-all-dev libssl-dev libz-dev
```

and then `cd` into the source directory and run

```
make
```

the binary will output to *./bin/Release*

**Windows**

First, clone the win32 git repo

```
git clone https://github.com/vexyl/MCHawk_win32
```

Place the MCHawk_win32 folder into MCHawk's project folder and rename it to win32.

Open the VS2017 project *MCHawk/win32/MCHawk.sln* and set the configuration to Release x86.

Build the project and copy contents from MCHawk/win32/Release to MCHawk/bin/Release

The same applies to Debug with the appropriate filename changes to the above instructions.

## Misc

The release version sends a heartbeat request to http://www.classicube.net by default. Edit *./bin/Release/config.ini* to change it.
