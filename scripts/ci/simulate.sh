#!/bin/sh

set -e

# Run
docker run --rm -v "$(pwd):/data" -w "/data" debian:buster-slim sh -c "./scripts/ci/run.sh"
