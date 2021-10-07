# mcpi-packages
This is a apt repo for various important MCPI Revival projects like [mcpi-reborn](https://gitea.thebrokenrail.com/TheBrokenRail/minecraft-pi-reborn) and [jmcpil](https://github.com/MCPI-Revival/jMCPIL)/[gMCPIL](https://github.com/MCPI-Revival/jMCPIL). The apt packages and package lists is stored in the [debs](debs/) and [bullseye-debs](bullseye-debs/) folder. Currently, the repo supports Buster and Bullseye systems.

### APT Repo Installation (for both x86 and arm)
```bash
wget -qO- https://raw.githubusercontent.com/MCPI-Revival/mcpi-packages/master/install.sh | bash
```

#### Uninstall
```bash
wget -qO- https://raw.githubusercontent.com/MCPI-Revival/mcpi-packages/master/uninstall.sh | bash
```

Currently the only packages available are:
- x86 and arm versions of `client` and `server` for `mcpi-reborn`
- `MCPIL` (legacy, not supported)
- `gmcpil`
- `jmcpil`
- Other random projects we have

### Submit your MCPI mod
This repo isn't just for MCPI Revival projects! If YOU have a cool python mod/program for mcpi, just package it up as a .deb file and make a issue to submit it so it can be added to our apt repo!

We reccomend following https://stackoverflow.com/a/26083895 for instructions on how to package a .py into a deb. Make sure that you use the format of `mcpi-<packagename>` in the name field of the CONTROL file when packaging to .deb!
