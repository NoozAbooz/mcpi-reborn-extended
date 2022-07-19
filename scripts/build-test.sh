#!/bin/bash

set -e

function error {
  echo -e "\e[91m$1\e[39m"
  exit 1
}

docker rm -f $(docker ps -a -q) || true

docker run --rm -v "$(pwd):/data" -w "/data" debian:buster-slim sh -c "./scripts/install-dependencies.sh && ./scripts/package.sh client amd64"

./out/minecraft-pi-reborn-client-*-amd64.AppImage
