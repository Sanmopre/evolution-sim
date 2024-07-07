#!/bin/bash

# Get the directory of the current script
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# Default build mode to Release if not specified
BUILD_MODE=${BUILD_MODE:-Release}

docker build --build-arg BUILD_MODE="$BUILD_MODE" -t application -f "$SCRIPT_DIR/../docker/Dockerfile" "$SCRIPT_DIR/.."