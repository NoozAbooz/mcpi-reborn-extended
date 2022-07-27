#!/bin/bash

set -e

# Prepare
NAME='minecraft-pi-reborn'
VERSION="$(cat VERSION)"

# Build
./scripts/setup.sh "$1" "$2" -DMCPI_IS_APPIMAGE_BUILD=ON
./scripts/build.sh "$1" "$2"

# Package AppImage
mkdir -p build/appimage
if [ ! -f "build/appimage/runtime-$2" ]; then
    case "$2" in
        'armhf') RUNTIME_ARCH='armhf';;
        'arm64') RUNTIME_ARCH='aarch64';;
        'i386') RUNTIME_ARCH='i686';;
        'amd64') RUNTIME_ARCH='x86_64';;
    esac
    wget -O "build/appimage/runtime-$2" "https://github.com/AppImage/AppImageKit/releases/download/continuous/runtime-${RUNTIME_ARCH}"
fi

# Package
case "$2" in
    'armhf') APPIMAGE_ARCH='arm';;
    'arm64') APPIMAGE_ARCH='arm_aarch64';;
    'i386') APPIMAGE_ARCH='i686';;
    'amd64') APPIMAGE_ARCH='x86_64';;
esac
ARCH="${APPIMAGE_ARCH}" appimagetool \
    --updateinformation "gh-releases-zsync|NoozSBC|mcpi-reborn-extended|latest|${NAME}-$1-$2.AppImage.zsync" \
    --runtime-file "build/appimage/runtime-$2" \
    --comp xz \
    "./out/$1-$2" \
    "./out/${NAME}-$1-$(cat VERSION)-$2.AppImage"

# Move ZSync
rm -f "./out/${NAME}-$1-$2.AppImage.zsync"
mv "./${NAME}-$1-$(cat VERSION)-$2.AppImage.zsync" "./out/${NAME}-$1-$2.AppImage.zsync"

# -----------------------------

# Package DEBs
outdir="./out/$1-$2"
pkg="$(basename "${outdir}")"

rm -rf "${outdir}/.DirIcon" "${outdir}/AppRun" ${outdir}/com.nooz.*

# Create DEBIAN Dir
rm -rf "${outdir}/DEBIAN"
mkdir -p "${outdir}/DEBIAN"
cp "./debian/$1-$2" "${outdir}/DEBIAN/control"

mv ${outdir}/lib ${outdir}/usr/lib

if [[ "$1" =~ .*"client".* ]]; then
    cp "debian/postinst" "${outdir}/DEBIAN/postinst"
    cp "debian/libminecraftpe.so" "${outdir}/usr/lib/minecraft-pi-reborn-client/game/libminecraftpe.so"

    ln -sf ../usr/lib/minecraft-pi-reborn-client/launcher ${outdir}/bin/minecraft-pi-reborn-client
fi
if [[ "$1" =~ .*"server".* ]]; then
    ln -sf ../usr/lib/minecraft-pi-reborn-server/launcher ${outdir}/bin/minecraft-pi-reborn-server
fi

# Format DEBIAN/control
sed -i "s/\${VERSION}/${VERSION}/g" "${outdir}/DEBIAN/control"

# Fix Permissions On Jenkins
sudo chmod -R g-s "${outdir}"

# Package
dpkg-deb --root-owner-group --build "${outdir}" out
