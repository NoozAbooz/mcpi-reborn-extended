# MCPI++ Source Code

The `main` branch is for hosting the apt repo and debs. This branch is for the source code used to compile the modded Appimages (a custom script is used for converting the Appimages to debs and publishing them).

## Compiling

### Depends
- Debian-based build enviroment (or Docker if you don't have one)
- git

Download the source code:
```
git clone --recurse-submodules -b source https://github.com/mobilegmYT/mcpi-reborn-extended
```

Make sure your OS is x86_64 and Debian Bullseye based!
To build everything for all available architectures use this:
```
ARM_PACKAGES_SUPPORTED=1 ./scripts/install-dependencies.sh
bash ./scripts/package-all.sh
```
If you are doing a lot of testing are you don't want to take the time to build everything then you will need to install dependencies with this command:
`bash ./scripts/install-dependencies.sh`

and then use this command each time you compile:

`./scripts/build.sh <client|server> <amd64|i386|arm64|armhf>`

package your built binaries with:

`./scripts/package.sh <client|server> <amd64|i386|arm64|armhf>`

You will need to choose your architecture and if you want to build the client or the server, click [here](https://github.com/mobilegmYT/mcpi-reborn-extended/blob/source/docs/INSTALL.md#picking-a-package) for more information.

Build output is in the out/ folder.
