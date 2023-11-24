#!/bin/bash

set -e

function error {
  echo -e "\e[91m$1\e[39m"
  exit 1
}

docker rm -f $(docker ps -a -q) || true

docker run --rm -v "$(pwd):/data" -w '/data' noozsbc/mcpi-reborn-extended-build:busterv2 ./scripts/package.sh client amd64

#./out/minecraft-pi-reborn-client-*-amd64.AppImage
