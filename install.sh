#!/bin/bash

# Define functions
function error() {
  echo -e "$(tput setaf 1)$(tput bold)$1$(tput sgr 0)"
  exit 1
}

function warning() {
  echo -e "$(tput setaf 1)$(tput bold)$1$(tput sgr 0)"
  sleep 2
  true
}

function install-depends() {
  if [ $(dpkg-query -W -f='${Status}' wget 2>/dev/null | grep -c "ok installed") -eq 0 ];
  then
    sudo apt-get install -y wget;
  fi

  if [ $(dpkg-query -W -f='${Status}' gnupg2 2>/dev/null | grep -c "ok installed") -eq 0 ];
  then
    sudo apt-get install gnupg2;
  fi
}

function install-api() {
  if [ $(dpkg-query -W -f='${Status}' python3-minecraftpi 2>/dev/null | grep -c "ok installed") -eq 0 ];
  then
    sudo apt-get install -y python3-minecraftpi;
  fi
  
  sudo sed -i -e '332d' /usr/lib/python3/dist-packages/mcpi/minecraft.py
  sudo sed -i '/def getPlayerEntityId(self, name):/i \
          if not len(ids) == 0:\
              return list(map(int, ids.split("|")))\
          else:\
              return []\

  ' minecraft.py
  sudo sed -i -e '332d' /usr/lib/python3/dist-packages/mcpi/minecraft.py
}

# Install depends if not already installed
read -r -p "Install dependencies wget and gnupg2? [y/N] " response
if [[ "$response" =~ ^([yY][eE][sS]|[yY])$ ]]
then
    install-depends()
else
    echo "Need depends to install! Exiting..." && exit 1
fi

# Install repo
wget -qO- https://github.com/mobilegmYT/mcpi-packages-extended/raw/main/debs/KEY.gpg | sudo apt-key add - || error "Failed to download and add key!"
wget -qO- https://github.com/mobilegmYT/mcpi-packages-extended/raw/main/debs/mcpi-revival.list | sudo tee /etc/apt/sources.list.d/mcpi-reborn-extended.list || error "Failed to download 'mcpi-revival.list'!"

sudo apt update --allow-releaseinfo-change || warning "Failed to run 'sudo apt update'! Please run that command manually"

# Nuke vanilla reborn if installed
if [ $(dpkg-query -W -f='${Status}' minecraft-pi-reborn-client 2>/dev/null | grep -c "ok installed") -eq 0 ];
then
  sudo apt-get uninstall -y minecraft-pi-reborn-client && sudo apt-get uninstall -t minecraft-pi-reborn-client; || warning "Could not reinstall reborn to switch to extended version! Please do it manually."
fi

# Install modified python lib
read -p " (y/n) " -n 1 -r
echo    # (optional) move to a new line
if [[ $REPLY =~ ^[Yy]$ ]]
then
  install-api()
fi

read -r -p "Install MCPI python API? [y/N] " response
if [[ "$response" =~ ^([yY][eE][sS]|[yY])$ ]]
then
    install-api()
fi

wget https://archive.org/download/libminecraftpe0.6.1/libminecraftpe06%2B08.so || warning "Failed to download sound files!"
mkdir -p ~/.minecraft-pi/overrides
mv libminecraftpe06+08.so ~/.minecraft-pi/overrides/libminecraftpe.so

echo "Installation success! Enjoy the game!"
