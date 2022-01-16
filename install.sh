#!/bin/bash

function error() {
  echo -e "$(tput setaf 1)$(tput bold)$1$(tput sgr 0)"
  exit 1
}

function warning() {
  echo -e "$(tput setaf 1)$(tput bold)$1$(tput sgr 0)"
  sleep 2
  true
}

# Install wget if not already installed
if ! command -v wget > /dev/null;then
  
  if [ -f /usr/bin/apt ];then
    sudo apt install -y wget || error "Failed to install wget package!"
  else
    error "Failed to find any package manager to install wget. Please make sure your are using a Debian-based OS!"
  fi
fi

sudo apt install gnupg2 -y

wget -qO- https://github.com/mobilegmYT/mcpi-packages-buster/raw/main/debs/KEY.gpg | sudo apt-key add - || error "Failed to download and add key!"
wget -qO- https://github.com/mobilegmYT/mcpi-packages-buster/raw/main/debs/mcpi-revival.list | sudo tee /etc/apt/sources.list.d/mcpi-packages-buster.list || error "Failed to download 'mcpi-revival.list'!"

sudo apt update --allow-releaseinfo-change || warning "Failed to run 'sudo apt update'! Please run that command manually"

# Nuke bullseye reborn if installed
if command -v minecraft-pi-reborn-client > /dev/null;then
  sudo apt remove -y minecraft-pi-reborn-client
  sudo apt install -y minecraft-pi-reborn-client
fi
