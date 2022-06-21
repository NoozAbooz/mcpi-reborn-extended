#!/bin/bash

read -p $'This script will REMOVE the MCPI++ APT REPO! Press [Enter] to continue, or [Ctrl+C] to exit\x0a'

sudo rm /etc/apt/sources.list.d/mcpi-reborn-extended.list
sudo rm /etc/apt/trusted.gpg.d/mcpi-reborn-extended.gpg
sudo apt update

sudo rm /tmp/mcpi-reborn-extended.gpg

read -p "Would you like to uninstall the MCPI++ application itself? You'll need to manually install vanilla reborn after. " -n 1 -r

echo  # (optional) move to a new line
if [[ $REPLY =~ ^[Yy]$ ]]
then
    sudo apt-get remove -y minecraft-pi-reborn-client
    sudo apt-get remove -y minecraft-pi-reborn-server
fi
