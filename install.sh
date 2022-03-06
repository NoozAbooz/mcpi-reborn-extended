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
sudo wget -qO- /etc/apt/sources.list.d/mcpi-reborn-extended.list https://github.com/mobilegmYT/mcpi-reborn-extended/raw/main/debs/mcpi-reborn-extended.list || error "Failed to download repo!"

# Install GPG key
sudo wget https://github.com/mobilegmYT/mcpi-reborn-extended/raw/main/debs/KEY.gpg
sudo apt-key add KEY.gpg
sudo rm KEY.gpg
#sudo mv KEY.gpg /etc/apt/trusted.gpg.d/mcpi-reborn-extended.gpg

sudo apt update --allow-releaseinfo-change || warning "Failed to run 'sudo apt update'! Please run that command manually"

# Nuke vanilla reborn if installed
if [ $(dpkg-query -W -f='${Status}' minecraft-pi-reborn-client 2>/dev/null | grep -c "ok installed") -eq 1 ];
then
  echo -e "\e[4m\e[21m\e[5mRemoving vanilla reborn to resolve conflicts...\e[0m\e[97m"
  sudo apt-get remove -y minecraft-pi-reborn-client || warning "Could not reinstall reborn to switch to extended version! Please do it manually"
  sudo apt-get install -y minecraft-pi-reborn-client
fi

# Install Python API
if [ $(dpkg-query -W -f='${Status}' python3-minecraftpi 2>/dev/null | grep -c "ok installed") -eq 0 ];
then
  echo "[OPTIONAL] Install Python API? (press '1' or '2' and then press [Enter] to select)"
  select yn in "Yes" "No"; do
    case $yn in
      Yes ) echo -e "\e[4m\e[21m\e[5m[OPTIONAL] Installing Python API...\e[0m\e[97m" && sudo apt install -y python3-minecraftpi || warning "Could not install Python API" && wget https://gist.github.com/mobilegmYT/78f50d3b80924d0c18ed818552254695/raw/a80ead7d30edf16327622002466f7b7e7df69aa6/minecraft.py && sudo mv minecraft.py /usr/lib/python3/dist-packages/mcpi/minecraft.py;;
      No ) break;;
    esac
  done
fi

# Install sound
echo "[OPTIONAL] Enable sound?"
select yn in "Yes" "No"; do
  case $yn in
    Yes ) echo -e "\e[4m\e[21m\e[5m[OPTIONAL] Installing sound files...\e[0m\e[97m" && wget https://archive.org/download/libminecraftpe0.6.1/libminecraftpe06%2B08.so && mkdir -p ~/.minecraft-pi/overrides && mv "libminecraftpe06+08.so" ~/.minecraft-pi/overrides/libminecraftpe.so;;
    No ) break;;
  esac
done

# Install 1.18 textures
function install_updated_textures {
  wget https://cdn.discordapp.com/attachments/740287938453045401/944751207644278864/converted.zip
  unzip converted.zip
  mkdir -p ~/.minecraft-pi/overrides/images
  mv converted/* ~/.minecraft-pi/overrides/images/
  rm -rf converted/ converted.zip
}

echo "[OPTIONAL] Install MC 1.18 texturepack?"
select yn in "Yes" "No"; do
  case $yn in
    Yes ) echo -e "\e[4m\e[21m\e[5m[OPTIONAL] Installing 1.18 texturepack...\e[0m\e[97m" && install_updated_textures;;
    No ) break;;
  esac
done

# Add custom skin
function convert_skin {
  pip install pillow
  skinpath=$(zenity --file-selection --title='Select your custom Java/Bedrock skin file (must be 4px)' --file-filter='Picture files (png) | *.png' --file-filter='All files | *')
  echo "Using image:" $skinpath
  
  cp $skinpath ~/.minecraft-pi/skin-mcpi.png
  mkdir -p ~/.minecraft-pi/overrides/images/mob
  
  wget https://bitbucket.org/MattHawkinsUK/rpispy-misc/raw/master/minecraft/minecraft_skin_fixer.py
  sed -i "s|/opt/minecraft-pi/data/images/mob/char.png|~/.minecraft-pi/skin-mcpi.png|g" minecraft_skin_fixer.py
  python3 minecraft_skin_fixer.py
  
  mv ~/.minecraft-pi/skin-mcpi.png ~/.minecraft-pi/overrides/images/mob/char.png
  rm minecraft_skin_fixer.py
}

echo "[OPTIONAL] Use custom skin?"
select yn in "Yes" "No"; do
  case $yn in
    Yes ) echo -e "\e[4m\e[21m\e[5m[OPTIONAL] Adding custom skin...\e[0m\e[97m" && install_updated_textures;;
    No ) break;;
  esac
done

echo "Installation success! Launch it with 'minecraft-pi-reborn-client' and enjoy!"
