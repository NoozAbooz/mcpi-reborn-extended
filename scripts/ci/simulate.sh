#!/bin/sh

set -e

# Run
#docker run --rm -v "$(pwd):/data" -w "/data" debian:buster-slim sh -c "./scripts/ci/run.sh"

docker run --rm -v "$(pwd):/data" -w '/data' -u "$(id -u):$(id -g)" noozsbc/mcpi-reborn-extended-build:latest ./scripts/ci/run.sh
