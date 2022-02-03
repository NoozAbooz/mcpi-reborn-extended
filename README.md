# mcpi-reborn-extended source code

The main branch is for the apt repo and debs. This branch is for the source code I use to compile the modded debs.

## Compiling
Download and unzip https://github.com/mobilegmYT/mcpi-reborn-extended/archive/refs/heads/source.zip, then open the resulting folder in terminal.

Make sure your OS is x86_64 and Debian Bullseye based!

```
ARM_PACKAGES_SUPPORTED=1 scripts/install-dependencies.sh
bash scripts/ci/build-all.sh
```

### Building for Debian Buster
We encourage you to use Debian Bullseye instead of Buster, but support is still provided for it. Your host OS can be either Bullseye or Buster for this, but it also needs to be x86_64.
Prerequisites:
- Docker (Use Docker Desktop set to Linux container mode with WSL2 if you are on Windows!)

```
ARM_PACKAGES_SUPPORTED=1 scripts/install-dependencies.sh
bash scripts/ci/simulate.sh
```

Build output is in the out/ folder.
