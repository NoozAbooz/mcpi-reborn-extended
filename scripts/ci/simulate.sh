#!/bin/sh

set -e

# Run
#docker run --rm -v "$(pwd):/data" -w "/data" debian:buster-slim sh -c "./scripts/ci/run.sh"

docker run --rm -v "$(pwd):/data" -w "/data" noozsbc/mcpi-reborn-extended-build:latest sh -c "./scripts/ci/run.sh"
