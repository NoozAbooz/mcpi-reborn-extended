# mcpi-packages
An APT repo for MCPI-Revival projects

## Installation
```bash
curl -s --compressed "deb https://mcpirevival.tk/mcpi-packages/debs/KEY.gpg" | sudo apt-key add -
sudo curl -s --compressed -o /etc/apt/sources.list.d/mcpi-revival.list "deb https://mcpirevival.tk/mcpi-packages/debs/mcpi-revival.list"
sudo apt update
```

Currently the only debs available are:
- `virgl`, `native` and `server` versions of `mcpi-reborn`
- `MCPIL`
