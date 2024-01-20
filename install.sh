#!/bin/bash

APT_URL="https://github.com/NoozSBC/mcpi-reborn-extended/raw/main/debs/mcpi-reborn-extended.list"
APT_PATH="/etc/apt/sources.list.d/mcpi-reborn-extended.list"
KEY_URL="https://github.com/NoozSBC/mcpi-reborn-extended/raw/main/debs/KEY.gpg"
KEY_PATH="/tmp/mcpi-reborn-extended.gpg"
API_URL="https://raw.githubusercontent.com/martinohanlon/mcpi/0c6ff49377da740c707e8c21b616f7fb21e6e85f/mcpi/minecraft.py"
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
  echo -e "\nAsk for support at https://discord.gg/XJJNG9jTuh"
  exit 1
}

sudo_wget() {
  sudo wget "$1" -O "$2" || error "Couldn't download '$2' from '$1'"
}

PAGE_SIZE="$(getconf PAGE_SIZE)"
if [[ "$PAGE_SIZE" == "16384" ]]; then
    #switch to 4K pagesize kernel
    if [ -f /boot/config.txt ]; then
        error "Raspberry Pi 5 PiOS images ship by default with a 16K PageSize Linux Kernel.
        This kernel causes incompatibilities with some software including MCPI++ https://github.com/raspberrypi/bookworm-feedback/issues/107

        To use it, you must switch to a 4K PageSize Linux Kernel. Enable it by adding 'kernel=kernel8.img' to /boot/config.txt. Reboot afterwards."
    fi
fi

# Install depends
#echo -e "\e[4m\e[21m\e[5mInstalling dependencies...\e[0m\e[97m"
info "Installing Dependencies..."
sudo apt-get install -y wget gnupg || error "Failed to install dependencies!"

# Install repo
info "Installing APT Repository..."
sudo_wget "${APT_URL}" "${APT_PATH}" || error "Could not download repo!"

# Install GPG key
info "Downloading APT Repository Signing Key..."
sudo_wget "${KEY_URL}" "${KEY_PATH}" || error "Could not download GPG key!"
sudo apt-key add "${KEY_PATH}"

info "Syncing APT Sources..."
sudo apt update --allow-releaseinfo-change || error "Failed to run 'sudo apt update'!"

# Install packages
if [[ "$1" == "--server" ]]; then
    info "Installing Server... This will allow you to host a server, but the game client itself is not installed. "
    sudo apt-get install -y minecraft-pi-reborn-server
    APP="server"
else
    info "Installing Game Client..."
    sudo apt-get install -y minecraft-pi-reborn-client
    APP="client"
fi

# Install launcher
#if [[ $APP == "client" ]]; then
#    echo -e "\nWould you like to install Planet Launcher? This will allow you to customize your skin and add texturepacks. [Y/n]"
#    read -p "> " -n 1 -r
#
#    if [[ $REPLY =~ ^[Yy]$ ]]; then
#        info "Installing Launcher..."
#        sudo apt-get install -y planet-launcher
#    fi
#fi

# Install Python API
info "Installing Python API..."
sudo apt install -y python3-minecraftpi || warning "Could not install Python API"
sudo_wget "${API_URL}" "${API_PATH}"

# Finish
echo -e "\n"
info "Installation complete! Refer to https://github.com/NoozSBC/mcpi-reborn-extended#user-guide for usage instructions."

# Self destruct
unlink "$0"
