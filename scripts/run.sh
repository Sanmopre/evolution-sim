# Run the Docker container with automatic cleanup
docker run --rm --name application_container --init -it -e TERM=xterm-256color application