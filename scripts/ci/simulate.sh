#!/bin/sh

set -e

# Run
docker run --rm -v "$(pwd):/data" -u '1000:1000' debian:buster-slim sh -c "cd /data; ./scripts/ci/run.sh"
