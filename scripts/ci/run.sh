#!/bin/sh

set -e

# Get Depends
#echo '==== Installing Dependencies ===='
#./scripts/install-dependencies.sh amd64 armhf arm64

# Build/Package
echo '==== Building & Packaging ===='

cd /data/dependencies/qemu/src/
./configure --with-git-submodules=validate
cd /data/

./scripts/package-all.sh

# Test
#echo '==== Testing ===='
#./scripts/test.sh
