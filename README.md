# mcpi-reborn-extended source code

The main branch is for the apt repo and debs themselves. This branch is for the source code I use to compile the custom buster debs.

## Compiling
Prerequisites:
- Docker (Use Docker Desktop set to Linux container mode with WSL2 if you are on Windows!)

Download and unzip https://github.com/mobilegmYT/mcpi-reborn-extended/archive/refs/heads/source.zip, then open the folder in terminal and run

```
ARM_PACKAGES_SUPPORTED=1 scripts/install-dependencies.sh
scripts/ci/simulate.sh
```

Build output is in the out/ folder.
