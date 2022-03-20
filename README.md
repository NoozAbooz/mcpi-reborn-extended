# MCPI++ Source Code

The `main` branch is for hosting the apt repo and debs. This branch is for the source code used to compile the modded debs.

## Compiling
### Depends
- Debian-based build enviroment (or Docker if you don't have one)
- git

Download the source code:
```
git clone --recurse-submodules -b source https://github.com/mobilegmYT/mcpi-reborn-extended
```

Make sure your OS is x86_64 and Debian Bullseye based! (for Buster scroll down)
To build everything for all available architectures use this:
```
./scripts/install-dependencies.sh
./scripts/ci/build-all.sh
```
If you are doing a lot of testing are you don't want to take the time to build everything then you will need to install dependencies with this command:
`sudo bash ./scripts/install-dependencies.sh`

and then use this command each time you compile:

`./scripts/build.sh <client|server> <amd64|arm64|armhf> && ./scripts/package.sh`

You will need to choose your architecture and if you want to build the client or the server, click [here](https://github.com/mobilegmYT/mcpi-reborn-extended/blob/source/docs/INSTALL.md#picking-a-package) for more info

### Building for Debian Buster
We encourage you to use Debian Bullseye instead of Buster, but support is still provided for it. Your host OS needs to be Buster-based for this, and it also needs to be x86_64. Make sure Docker is installed!

```
ARM_PACKAGES_SUPPORTED=1 scripts/install-dependencies.sh
bash scripts/ci/simulate.sh
```

Build output is in the out/ folder.