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

sudo wget -q -O /etc/apt/trusted.gpg.d/mcpi-packages-buster.gpg https://github.com/mobilegmYT/mcpi-packages-buster/raw/main/debs/KEY.gpg || error "Failed to download and add key!"
echo "deb [trusted=yes] https://mobilegmyt.github.io/mcpi-packages-buster/ /" | sudo tee /etc/apt/sources.list.d/mcpi-packages-buster.list || error "Failed to download 'mcpi-revival.list'!"

sudo apt update --allow-releaseinfo-change || warning "Failed to run 'sudo apt update'! Please run that command manually"
