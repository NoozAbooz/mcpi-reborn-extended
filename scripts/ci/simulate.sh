#!/bin/sh

set -e

# Run
docker run --rm -v "$(pwd):/data" debian:buster-slim sh -c "cd /data; ./scripts/ci/run.sh"
