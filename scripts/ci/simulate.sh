#!/bin/sh

set -e

# Run
docker run --rm -v "$(pwd):/data" debian:bullseye-slim sh -c "cd /data; ./scripts/ci/run.sh"
