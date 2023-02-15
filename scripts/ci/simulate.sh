#!/bin/sh

set -e

# Run
docker run --rm -v "$(pwd):/data" -w "/data" buildpack-deps:bullseye sh -c "./scripts/ci/run.sh"
