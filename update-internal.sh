#!/bin/bash

# Define variables
start=$(date +%s.%N)

function error() {
  echo -e "$(tput setaf 1)$(tput bold)$1$(tput sgr 0)"
  exit 1
}

convertsecs() {
 h=$(bc <<< "${1}/3600")
 m=$(bc <<< "(${1}%3600)/60")
 s=$(bc <<< "${1}%60")
 printf "%02d.%05.2f\n" $m $s
}

# Run pull latest code from git
cd ~/minecraft-pi-reborn
git pull || error "Couldn't merge with remote!"
LATEST_COMMIT="$(git rev-parse HEAD)"
git add --all
git commit -m "$1"
git push

# Build reborn
if [ -z "$NOBUILD" ]; then
  ./scripts/ci/simulate.sh || error "Build failed! Not pushing debs."
  #sudo scripts/package-debs.sh || error "Package failed! Not pushing debs."
  sudo find ~/minecraft-pi-reborn/ -type d -exec chown mike {} \;
else
  echo "NOBUILD specified, continuing..."
fi

# Copy debs
VERSION="$(cat VERSION)"

cd ~/mcpi-reborn-extended/debs
rm *reborn*.deb

cp ~/minecraft-pi-reborn/out/*.deb .

sudo apt install -y ./minecraft-pi-reborn*amd64*.deb

# Upload appimages
cd ~/minecraft-pi-reborn/out/
DOTLESS_VERSION=$(echo $VERSION | sed -e 's/\.//g')
gh release create $VERSION --target source --notes "View the changelog for this release [here](https://github.com/mobilegmYT/mcpi-reborn-extended/blob/source/docs/CHANGELOG.md#$DOTLESS_VERSION). Check the [commit list](https://github.com/mobilegmYT/mcpi-reborn-extended/commits/source) for this version to see what source code was modified" *.AppImage* *.deb

# Planet
cd ~/mcpi-reborn-extended/debs
rm *planet*.deb

wget $(curl -s https://api.github.com/repos/mcpiscript/Planet/releases/latest | grep browser_download_url | cut -d '"' -f 4)

# Sign debs
dpkg-scanpackages --multiversion . > Packages
gzip -k -f Packages

apt-ftparchive release . > Release
gpg --batch --default-key "B3CBD723C2982EBDFEDCE197373BAC436E62C806" -abs -o - Release > Release.gpg
gpg --batch --default-key "B3CBD723C2982EBDFEDCE197373BAC436E62C806" --clearsign -o - Release > InRelease

# Push to github
cd ~/mcpi-reborn-extended/
git add --all
if [ -z "$1" ]; then
    
    export MSG="Update debs to source commit $LATEST_COMMIT"
else
    export MSG="$1"
fi
git pull
git commit -m "$MSG"
git push

echo "Pushed at $(date)"

duration=$(echo "$(date +%s.%N) - $start" | bc)

echo "Script Execution Time: $(convertsecs $duration) minutes"
