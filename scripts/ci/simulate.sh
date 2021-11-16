#!/bin/sh

set -e

# Run
docker run --rm -v "$(pwd):/data" bitnami/minideb:buster sh -c "cd /data; ./scripts/ci/run.sh"
