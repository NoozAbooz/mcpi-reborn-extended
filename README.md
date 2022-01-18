# mcpi-packages-buster source code

The main branch is for the apt repo and debs themselves. This branch is for the source code I use to compile the custom buster debs.

## Compiling
Prerequisites:
- Docker (Docker Desktop set to use WSL if on windows)

Download and unzip https://github.com/mobilegmYT/mcpi-packages-buster/archive/refs/heads/source.zip, then open the folder in terminal

```
ARM_PACKAGES_SUPPORTED=1 scripts/install-dependencies.sh
scripts/ci/simulate.sh
```

Build output is in the out/ folder.