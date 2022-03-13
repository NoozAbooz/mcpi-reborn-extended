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
if [ $(dpkg-query -W -f='${Status}' wget 2>/dev/null | grep -c "ok installed") -eq 0 ] || [ $(dpkg-query -W -f='${Status}' gnupg2 2>/dev/null | grep -c "ok installed") -eq 0 ] || [ $(dpkg-query -W -f='${Status}' unzip 2>/dev/null | grep -c "ok installed") -eq 0 ] || [ $(dpkg-query -W -f='${Status}' zenity 2>/dev/null | grep -c "ok installed") -eq 0 ];
then
  echo -e "\e[4m\e[21m\e[5mInstalling dependencies...\e[0m\e[97m"
  sudo apt install -y wget gnupg unzip zenity || error "Failed to install dependencies, see error above"
fi

# Install repo
echo -e "\e[4m\e[21m\e[5mInstalling APT repository...\e[0m\e[97m"
sudo wget /etc/apt/sources.list.d/mcpi-reborn-extended.list https://github.com/mobilegmYT/mcpi-reborn-extended/raw/main/debs/mcpi-reborn-extended.list

# Install GPG key
sudo wget https://github.com/mobilegmYT/mcpi-reborn-extended/raw/main/debs/KEY.gpg
sudo apt-key add KEY.gpg
sudo rm KEY.gpg
#sudo mv KEY.gpg /etc/apt/trusted.gpg.d/mcpi-reborn-extended.gpg

sudo apt update --allow-releaseinfo-change || warning "Failed to run 'sudo apt update'! Please run that command manually"

# Nuke vanilla reborn if installed
if [ $(dpkg-query -W -f='${Status}' minecraft-pi-reborn-client 2>/dev/null | grep -c "ok installed") -eq 1 ]
then
  echo -e "\e[4m\e[21m\e[5mRemoving vanilla reborn to resolve conflicts...\e[0m\e[97m"
  sudo apt-get remove -y minecraft-pi-reborn-client || warning "Could not reinstall reborn to switch to extended version! Please do it manually"
  sudo apt-get install -y minecraft-pi-reborn-client
fi

# Install Python API
if [ $(dpkg-query -W -f='${Status}' python3-minecraftpi 2>/dev/null | grep -c "ok installed") -eq 0 ]
then
  echo  -e "\033[33;5mInstalling Python API...\033[0m"
  sudo apt install -y python3-minecraftpi || warning "Could not install Python API"
  wget https://gist.github.com/mobilegmYT/78f50d3b80924d0c18ed818552254695/raw/a80ead7d30edf16327622002466f7b7e7df69aa6/minecraft.py
  sudo mv minecraft.py /usr/lib/python3/dist-packages/mcpi/minecraft.py
fi

# Install sound
echo  -e "\033[33;5mInstalling sound files...\033[0m"
wget https://archive.org/download/libminecraftpe0.6.1/libminecraftpe06%2B08.so
mkdir -p ~/.minecraft-pi/overrides
mv "libminecraftpe06+08.so" ~/.minecraft-pi/overrides/libminecraftpe.so

# Install 1.18 textures
echo  -e "\033[33;5mInstalling 1.18 textures...\033[0m"
wget https://cdn.discordapp.com/attachments/740287938453045401/944751207644278864/converted.zip
unzip converted.zip
mkdir -p ~/.minecraft-pi/overrides/images
mv converted/* ~/.minecraft-pi/overrides/images/
rm -rf converted/ converted.zip

# Add custom skin
echo  -e "\033[33;5mEnabling custom skins...\033[0m"
pip install pillow
skinpath=$(zenity --file-selection --title='Select your custom Java/Bedrock skin file (must be 4px). If you want to use the default skin, click cancel' --file-filter='Picture files (png) | *.png' --file-filter='All files | *')
echo "Using skin located at:" $skinpath
    
cp $skinpath ~/.minecraft-pi/skin-mcpi.png
mkdir -p ~/.minecraft-pi/overrides/images/mob
    
echo "Applying texture size patch..."
wget https://bitbucket.org/MattHawkinsUK/rpispy-misc/raw/master/minecraft/minecraft_skin_fixer.py
sed -i "s|/opt/minecraft-pi/data/images/mob/char.png|~/.minecraft-pi/skin-mcpi.png|g" minecraft_skin_fixer.py
python3 minecraft_skin_fixer.py
    
mv ~/.minecraft-pi/skin-mcpi.png ~/.minecraft-pi/overrides/images/mob/char.png
rm minecraft_skin_fixer.py


