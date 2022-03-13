#!/bin/sh

set -e

# Generate
./scripts/generate-appimage-builder-yaml.js "$1" "$2"

# Build/Package
appimage-builder --recipe AppImageBuilder.yml

# Move ZSync
mv "./minecraft-pi-reborn-$1-$(cat VERSION)-$2.AppImage.zsync" "./out/minecraft-pi-reborn-$1-$(cat VERSION)-$2.AppImage.zsync"