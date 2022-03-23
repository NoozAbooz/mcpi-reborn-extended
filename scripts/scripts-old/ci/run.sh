#!/bin/sh

set -e

./scripts/install-dependencies.sh

# Build/Package
echo '==== Building & Packaging ===='
./scripts/build-all.sh

# Test
echo '==== Testing ===='
./scripts/test.sh
