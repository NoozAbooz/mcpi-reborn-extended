# mcpi-packages
This is a apt repo for various important MCPI Revival projects like [mcpi-reborn](https://gitea.thebrokenrail.com/TheBrokenRail/minecraft-pi-reborn) and [mcpil](https://github.com/MCPI-Revival/MCPIL). The apt packages and package lists is stored in the [debs](debs/) folder.

### APT Repo Installation (for x86 and arm)
```bash
wget -qO- https://raw.githubusercontent.com/MCPI-Revival/mcpi-packages/master/install.sh | bash
```

#### Uninstall
```bash
wget -qO- https://raw.githubusercontent.com/MCPI-Revival/mcpi-packages/master/uninstall.sh | bash
```

Currently the only debs available are:
- x86 and arm versions of `client` and `server` for `mcpi-reborn`
- `MCPIL` (legacy, not supported)
- `gmcpil`
- `jmcpil`
- 

### Submit your MCPI mod
This repo isn't just for MCPI Revival projects! If YOU have a cool python mod/program for mcpi, just package it up as a .deb file and make a issue to submit it so it can be added to our apt repo!

We reccomend following https://stackoverflow.com/a/26083895 for instructions on how to package a .py into a deb. Make sure that you use the format of `mcpi-<packagename>` in the name field of the CONTROL file when packaging to .deb!
