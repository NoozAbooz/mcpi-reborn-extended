#!/bin/bash

APT_URL="https://github.com/NoozSBC/mcpi-reborn-extended/raw/main/debs/mcpi-reborn-extended.list"
APT_PATH="/etc/apt/sources.list.d/mcpi-reborn-extended.list"
KEY_URL="https://github.com/NoozSBC/mcpi-reborn-extended/raw/main/debs/KEY.gpg"
KEY_PATH="/tmp/mcpi-reborn-extended.gpg"
API_URL="https://github.com/martinohanlon/mcpi/blob/0c6ff49377da740c707e8c21b616f7fb21e6e85f/mcpi/minecraft.py"
API_PATH="/usr/lib/python3/dist-packages/mcpi/minecraft.py"

# Define functions
info() {
  echo -e "$(tput smul)$(tput setaf 6)$(tput bold)$1$(tput sgr0)"
}

warning() {
  echo -e "$(tput setaf 3)$(tput bold)$1$(tput sgr0)"
  sleep 2
  true
}

error() {
  echo -e "$(tput setaf 1)$(tput bold)$1$(tput sgr0)"
  exit 1
}

sudo_wget() {
  sudo wget "$1" -O "$2" || error "Couldn't download '$2' from '$1'"
}

# Install depends
#echo -e "\e[4m\e[21m\e[5mInstalling dependencies...\e[0m\e[97m"
info "Installing Dependencies..."
sudo apt-get install -y wget gnupg || error "Failed to install dependencies, see error above"

# Install repo
info "Installing APT Repository..."
sudo_wget "${APT_URL}" "${APT_PATH}"

# Install GPG key
info "Downloading APT Repository Key..."
sudo_wget "${KEY_URL}" "${KEY_PATH}"
sudo apt-key add "${KEY_PATH}"

info "Syncing APT Sources..."
sudo apt update --allow-releaseinfo-change || error "Failed to run 'sudo apt update'! Please check above errors"

# Install packages
echo -e "\nDo you want to install the client? If you choose no, then only the multiplayer server software will be installed. SAY NO ONLY IF YOU KNOW WHAT YOU ARE DOING! [Y/n]"
read -p "> " -n 1 -r

if [[ $REPLY =~ ^[Yy]$ ]]
then
    sudo apt-get install -y minecraft-pi-reborn-client
    info "Installed Client!"
    APP="client"
elif [[ $REPLY =~ ^[Nn]$ ]]
then
    sudo apt-get install -y minecraft-pi-reborn-server
    info "Installed Server! This will allow you to host a server, but the game itself is not installed. "
    APP="server"
fi

# Install launcher
if [[ $APP == "client" ]]
then
    echo -e "\nWould you like to install a launcher? This will allow you to save your settings and configure texturepacks/custom skins (RECCOMENDED). [Y/n]"
    read -p "> " -n 1 -r

    if [[ $REPLY =~ ^[Yy]$ ]]
    then
        info "Installing Launcher..."
        sudo apt-get install -y planet-launcher
    fi
fi

# Install Python API
info "Installing Python API..."
sudo apt install -y python3-minecraftpi || warning "Could not install Python API"
sudo_wget "${API_URL}" "${API_PATH}"

# Finish
echo -e "\n"
info "Installation complete! Refer to https://github.com/NoozSBC/mcpi-reborn-extended#user-guide for usage instructions."

# Self destruct
unlink $0
