#!/bin/sh

set -e

# Get Depends
#echo '==== Installing Dependencies ===='
#./scripts/install-dependencies.sh amd64 armhf arm64

# Build/Package
echo '==== Building & Packaging ===='

./scripts/package-all.sh

# Test
#echo '==== Testing ===='
#./scripts/test.sh
