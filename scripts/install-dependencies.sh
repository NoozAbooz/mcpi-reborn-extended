#!/bin/sh

set -e

# This Script Assumes An x86_64 Host
if [ "$(uname -m)" != "x86_64" ]; then
    echo 'Invalid Build Architecture'
    exit 1
fi


# Update APT
apt-get update
apt-get install -y sudo
sudo apt-get dist-upgrade -y

# Add ARM Repository
sudo dpkg --add-architecture armhf
sudo dpkg --add-architecture arm64

# Install Everything In One Go
PKG_QUEUE=''
queue_pkg() {
    PKG_QUEUE="${PKG_QUEUE} $@"
}

sudo apt update

# Build Tools
queue_pkg \
    ca-certificates \
    lsb-release \
    dpkg-dev \
    clang \
    lld \
    cmake \
    make \
    libglfw3 libglfw3-dev \
    libfreeimage3 libfreeimage-dev \
    crossbuild-essential-armhf \
    gcc g++ \
    libopenal-dev \
    qemu-user

# ARM Packages

## armhf
queue_pkg \
    libglfw3:armhf \
    libglfw3-dev:armhf \
    libfreeimage3:armhf \
    libopenal-dev:armhf \

## arm64
queue_pkg \
    libglfw3:arm64 \
    libglfw3-dev:arm64 \
    libfreeimage3:arm64 \
    libopenal-dev:arm64 \
    crossbuild-essential-arm64
    
# Install Queue
sudo apt-get install --no-install-recommends -y ${PKG_QUEUE}