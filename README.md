# MCPI++ Source Code

The [main](https://github.com/NoozSBC/mcpi-reborn-extended/tree/main) branch is for hosting the APT repo and debs. This branch is for the source code used to compile the modded debs.

If you are concerned about the mod having malware of any kind, just don't use it. All code is made available and you can compare the source code if you are technically-knowledged.

## Compiling
### Depends
- Debian-based build enviroment (or Docker if you don't have one)
- git

Download the source code:
```
git clone --recurse-submodules -b source https://github.com/NoozSBC/mcpi-reborn-extended && cd mcpi-reborn-extended
```

Make sure your OS is x86_64!
To build everything for all available architectures use this:
```
./scripts/install-dependencies.sh
./scripts/package-all.sh
```

If you are doing a lot of testing are you don't want to take the time to build everything then you will need to install dependencies with this command:
`sudo bash ./scripts/install-dependencies.sh`

and then use this command each time you compile:

`./scripts/package.sh <client|server> <amd64|arm64|armhf>`

Build output is in the out/ folder.
