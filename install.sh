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

if [[ "$(lsb_release -cs)" == "buster" ]]
then
  wget -qO- https://mobilegmyt.github.io/mcpi-packages/debs/KEY.gpg | sudo apt-key add - || error "Failed to download and add key!"
  sudo wget https://mobilegmyt.github.io/mcpi-packages/debs/mcpi-revival.list -O /etc/apt/sources.list.d/mcpi-revival.list || error "Failed to download 'mcpi-revival.list'!"
fi

sudo apt update || warning "Failed to run 'sudo apt update'! Please run that command manually"

if [[ "$(lsb_release -cs)" != "buster" && "$(lsb_release -cs)" != "bullseye" && "$(lsb_release -cs)" != "sid" && "$ID" != "ubuntu" ]]; then
  error "Unsupported OS Version! Your OS version is $(lsb_release -cs), but supported version is Buster. You may also be getting this if you are on a non-debian based OS!"
fi
