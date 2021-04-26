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

wget -qO- https://mcpirevival.tk/mcpi-packages/debs/KEY.gpg | sudo apt-key add - || error "Failed to download and add key!"
sudo wget https://mcpirevival.tk/mcpi-packages/debs/mcpi-revival.list -O /etc/apt/sources.list.d/mcpi-revival.list || error "Failed to download 'mcpi-revival.list'!"
sudo apt update || warning "Failed to run 'sudo apt update'! Please run that command manually"
