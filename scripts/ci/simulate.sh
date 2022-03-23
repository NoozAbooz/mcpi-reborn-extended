#!/bin/sh

set -e

# Run
docker run --rm -v "$(pwd):/data" -w "/data" debian:bullseye-slim sh -c "./scripts/ci/run.sh"
