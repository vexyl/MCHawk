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

First, download the project files and dependencies from [Dropbox](https://www.dropbox.com/s/9lultfjwq4362j6/MCHawk_win32.zip?dl=0)

Extract MCHawk_win32.zip into MCHawk's project folder. It should look like *MCHawk/win32*.

Open the VS2017 project *MCHawk/win32/MCHawk.sln* and set the configuration to Release x86.

Build the project and copy contents from MCHawk/win32/Release to MCHawk/bin/Release

The same applies to Debug with the appropriate filename changes to the above instructions.

## Misc

The release version sends a heartbeat request to http://www.classicube.net by default. Edit *./bin/Release/config.ini* to change it.


Discord: https://discord.gg/xwDejUa
