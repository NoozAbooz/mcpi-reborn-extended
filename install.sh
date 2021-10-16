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

#install wget if not already installed
if ! command -v wget >/dev/null;then
  
  if [ -f /usr/bin/apt ];then
    sudo apt install -y wget || error "Failed to install wget package!"
  else
    error "Failed to find any package manager to install wget. Please make sure your are using a Debian-based OS!"
  fi
fi

wget -qO- https://github.com/mobilegmYT/mcpi-packages-buster/raw/main/debs/KEY.gpg | sudo apt-key add - || error "Failed to download and add key!"
echo "deb [trusted=yes] https://mobilegmyt.github.io/mcpi-packages-buster/debs/ /" | sudo tee /etc/apt/sources.list.d/mcpi-packages-buster.list || error "Failed to download 'mcpi-revival.list'!"

sudo apt update --allow-releaseinfo-change || warning "Failed to run 'sudo apt update'! Please run that command manually"
