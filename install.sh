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

# Install depends
if [ $(dpkg-query -W -f='${Status}' wget 2>/dev/null | grep -c "ok installed") -eq 0 ]; 
then
  sudo apt-get install -y wget
fi 
  
if [ $(dpkg-query -W -f='${Status}' gnupg2 2>/dev/null | grep -c "ok installed") -eq 0 ];
then
  sudo apt-get install gnupg2
fi

if [ $(dpkg-query -W -f='${Status}' unzip 2>/dev/null | grep -c "ok installed") -eq 0 ];
then
  sudo apt-get install unzip
fi

# Install repo
sudo wget -q -O /etc/apt/sources.list.d/mcpi-reborn-extended.list https://github.com/mobilegmYT/mcpi-reborn-extended/raw/main/debs/mcpi-reborn-extended.list

# Install GPG key
sudo wget https://github.com/mobilegmYT/mcpi-reborn-extended/raw/main/debs/KEY.gpg
sudo apt-key add KEY.gpg
sudo rm KEY.gpg
#sudo mv KEY.gpg /etc/apt/trusted.gpg.d/mcpi-reborn-extended.gpg

sudo apt update --allow-releaseinfo-change || warning "Failed to run 'sudo apt update'! Please run that command manually"

# Nuke vanilla reborn if installed
if [ $(dpkg-query -W -f='${Status}' minecraft-pi-reborn-client 2>/dev/null | grep -c "ok installed") -eq 1 ];
then
  sudo apt-get remove -y minecraft-pi-reborn-client || warning "Could not reinstall reborn to switch to extended version! Please do it manually."
fi

sudo apt-get install -y minecraft-pi-reborn-client

# Install Python API
if [ $(dpkg-query -W -f='${Status}' python3-minecraftpi 2>/dev/null | grep -c "ok installed") -eq 0 ];
then
  sudo apt-get install -y python3-minecraftpi
fi 
  
wget https://gist.github.com/mobilegmYT/78f50d3b80924d0c18ed818552254695/raw/a80ead7d30edf16327622002466f7b7e7df69aa6/minecraft.py
sudo mv minecraft.py /usr/lib/python3/dist-packages/mcpi/minecraft.py

# Install sound
wget https://archive.org/download/libminecraftpe0.6.1/libminecraftpe06%2B08.so || warning "Failed to download sound files!"
mkdir -p ~/.minecraft-pi/overrides
mv libminecraftpe06+08.so ~/.minecraft-pi/overrides/libminecraftpe.so

# Install 1.18 textures
echo "The script will install a texturepack that updates the textures to MC Java 1.18! If you don't want this, run 'rm -rf ~/.minecraft-pi/overrides/images' after installation."
sleep 5
wget https://cdn.discordapp.com/attachments/740287938453045401/944751207644278864/converted.zip
unzip converted.zip
mkdir -p ~/.minecraft-pi/overrides/images
mv converted/* ~/.minecraft-pi/overrides/images/
rm -rf converted/ converted.zip

echo "Installation success! Launch it with 'minecraft-pi-reborn-client' and enjoy!"
