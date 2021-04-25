This is a apt repo for various important MCPI Revival projects like [mcpi-reborn](https://gitea.thebrokenrail.com/TheBrokenRail/minecraft-pi-reborn) and [mcpil](https://github.com/MCPI-Revival/MCPIL). All the apt repo stuff is stored in the [debs](debs/) folder.

## APT Repo Installation (works on x86 and arm)
```bash
wget -qO- https://raw.githubusercontent.com/MCPI-Revival/mcpi-packages/master/install.sh | bash
```

Currently the only debs available are:
- `virgl`, `native` and `server` versions of `mcpi-reborn`
- `MCPIL`
- `mcpirdl` (beta dockerless mcpi-reborn)
- `gmcpil`
- `jmcpil`
