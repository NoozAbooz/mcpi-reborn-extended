#!/bin/bash

read -p $'This script will REMOVE the MCPI++ APT REPO! Press [Enter] to continue, or [Ctrl+C] to exit\x0a'

sudo rm /etc/apt/sources.list.d/mcpi-reborn-extended.list
sudo rm /etc/apt/trusted.gpg.d/mcpi-reborn-extended.gpg
sudo apt update

sudo rm /tmp/mcpi-reborn-extended.gpg

echo -e "\nWould you like to uninstall the MCPI++ application itself? [Y/n]"
read -p "> " -n 1 -r

if [[ $REPLY =~ ^[Yy]$ ]]
then
    sudo apt-get remove -y minecraft-pi-reborn-client
    sudo apt-get remove -y minecraft-pi-reborn-server
fi

echo -e "\nUninstall complete! Please note that the MCPI Python API is still installed and can be removed with 'pip uninstall mcpi'. Your user data folder, '~/.minecraft-pi/' can also be removed if you want."

# Self destruct
unlink $0
