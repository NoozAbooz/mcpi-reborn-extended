#!/bin/sh

set -e

ARM_PACKAGES_SUPPORTED=1 ./scripts/install-dependencies.sh

# Build/Package
echo '==== Building & Packaging ===='
./scripts/package-all.sh

# Test
echo '==== Testing ===='
./scripts/test.sh
