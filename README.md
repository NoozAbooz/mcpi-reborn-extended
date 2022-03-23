# MCPI++ Source Code

The `main` branch is for hosting the apt repo and debs. This branch is for the source code used to compile the modded debs.

## Compiling
### Depends
- Debian-based build enviroment (or Docker if you don't have one)
- git

Download the source code:
```
git clone --recurse-submodules -b source https://github.com/mobilegmYT/mcpi-reborn-extended && cd mcpi-reborn-extended
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

You will need to choose your architecture and if you want to build the client or the server, click [here](https://github.com/mobilegmYT/mcpi-reborn-extended/blob/source/docs/INSTALL.md#picking-a-package) for more info

### Building on Windows
If you are a windows user on WSL and want to build packages, you'll need to have Docker Desktop installed and set to the WSL backend.

```
./scripts/ci/simulate.sh
```

Build output is in the out/ folder.