#!/bin/sh

set -e

# Package Debs
VERSION="$(cat VERSION)"

package() {
    local dir="out/$1"

    # Create DEBIAN Dir
    rm -rf "${dir}/DEBIAN"
    mkdir -p "${dir}/DEBIAN"
    cp "debian/$1" "${dir}/DEBIAN/control"
    cp "debian/postinst" "${dir}/DEBIAN/control"

    # Format DEBIAN/control
    sed -i "s/\${VERSION}/${VERSION}/g" "${dir}/DEBIAN/control"

    # Fix Permissions On Jenkins
    sudo chmod -R g-s "${dir}"

    # Package
    dpkg-deb --root-owner-group --build "${dir}" out
}

# Find And Package
for dir in out/*; do
    # Check If Directory Exists
    if [ -d "${dir}" ]; then
        # Check If Debian Package Exists
        pkg="$(basename ${dir})"
        if [ -f "debian/${pkg}" ]; then
            package "${pkg}"
        fi
    fi
done
