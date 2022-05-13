#!/bin/bash

MCPI_DIR="${HOME}/.minecraft-pi"
LIST_URL="https://github.com/mobilegmYT/mcpi-reborn-extended/raw/main/debs/mcpi-reborn-extended.list"
LIST_PATH="/etc/apt/sources.list.d/mcpi-reborn-extended.list"
KEY_URL="https://github.com/mobilegmYT/mcpi-reborn-extended/raw/main/debs/KEY.gpg"
KEY_PATH="/tmp/mcpi-reborn-extended.gpg"
MCPI_PY_URL="https://gist.github.com/mobilegmYT/78f50d3b80924d0c18ed818552254695/raw/a80ead7d30edf16327622002466f7b7e7df69aa6/minecraft.py"
MCPI_PY_PATH="/usr/lib/python3/dist-packages/mcpi/minecraft.py"
SOUND_URL="https://archive.org/download/libminecraftpe0.6.1/libminecraftpe06%2B08.so"
SOUND_PATH="${MCPI_DIR}/overrides/libminecraftpe.so"
TEXTURES_PREFIX="converted"
TEXTURES_URL="https://cdn.discordapp.com/attachments/896528332340998174/956268263036321842/converted.zip"
TMP_TEXTURES_PATH="/tmp/mcpi-reborn-textures.zip"
TEXTURES_PATH="${MCPI_DIR}/overrides/images"
MOB_TEXTURES_PATH="${TEXTURES_PATH}/mob"
FIXER_PATH="/tmp/mcpi-skin-fixer.py"
TMP_SKIN_PATH="${MCPI_DIR}/skin-mcpi.png"

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
  exit 1
}

priv_wget() {
  sudo wget "$1" -O "$2" || error "Couldn't download '$2' from '$1'"
}

skin_dialog() {
  zenity --file-selection --file-filter="Picture files (png) | *.png" --file-filter="All files | *" \
    --title="Select your custom Java/Bedrock skin file (must be 4px). If you want to use the default Steve skin, close this window"
}

# Install depends
echo -e "\e[4m\e[21m\e[5mInstalling dependencies...\e[0m\e[97m"
sudo apt install -y wget gnupg unzip zenity || error "Failed to install dependencies, see error above"

# Install repo
info "Installing APT repository..."
priv_wget "${LIST_URL}" "${LIST_PATH}"

# Install GPG key
priv_wget "${KEY_URL}" "${KEY_PATH}"
sudo apt-key add "${KEY_PATH}"

sudo apt update --allow-releaseinfo-change || warning "Failed to run 'sudo apt update'! Please run that command manually"

# Nuke vanilla reborn if installed
info "Removing vanilla reborn to resolve conflicts..."
sudo apt-get remove -y minecraft-pi-reborn-client || warning "Could not reinstall reborn to switch to extended version! Please do it manually"
sudo apt-get install -y minecraft-pi-reborn-client planet-launcher

# Install Python API
info "Installing Python API..."
sudo apt install -y python3-minecraftpi || warning "Could not install Python API"
priv_wget "${MCPI_PY_URL}" "${MCPI_PY_PATH}"

mkdir ~/.minecraft-pi/overrides/
mkdir ~/.minecraft-pi/overrides/images/
mkdir -p "${MOB_TEXTURES_PATH}"

# Install sound
info "Installing sound files..."
wget "${SOUND_URL}" -O "${SOUND_PATH}"

# Install 1.18 textures
info "Installing 1.18 textures..."
(
  set -e
  cd /tmp
  wget "${TEXTURES_URL}" -O "${TMP_TEXTURES_PATH}"
  unzip "${TMP_TEXTURES_PATH}"
  rm -rf "${TEXTURES_PATH}/*"
  sudo mv "converted/"* "${TEXTURES_PATH}"
)

# Add custom skin
info "Enabling custom skins..."
pip install pillow
pip3 install pillow
SKIN_PATH="$(skin_dialog)"
echo "Using skin located at '${SKIN_PATH}'"

sudo cp "${SKIN_PATH}" "${TMP_SKIN_PATH}"
    
echo "Applying texture size patch..."
(
  set -e
  cd /tmp
  wget "https://bitbucket.org/MattHawkinsUK/rpispy-misc/raw/master/minecraft/minecraft_skin_fixer.py" -O "${FIXER_PATH}"
  sed -i "s|/opt/minecraft-pi/data/images/mob/char.png|${TMP_SKIN_PATH}|g" "${FIXER_PATH}"
  sudo python3 "${FIXER_PATH}"
)
    
sudo mv "${TMP_SKIN_PATH}" "${MOB_TEXTURES_PATH}/char.png"

# Finish
echo -e "\n"
info "Installation complete! Refer to https://github.com/mobilegmYT/mcpi-reborn-extended#user-guide for usage instructions."
