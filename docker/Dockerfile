FROM ubuntu:latest
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && \
    apt-get install -y cmake build-essential && \
    apt-get install -y clang-format && \
    apt-get install -y libgtest-dev && \
    apt-get install -y doxygen && \
    apt-get install -y clang-tidy
WORKDIR /app
COPY . /app
ARG BUILD_MODE=Release
RUN cmake -DCMAKE_BUILD_TYPE=${BUILD_MODE} -DBUILD_TESTS=OFF -DUSE_CLANG_FORMAT=OFF -DUSE_CLANG_TIDY=OFF . && make
CMD ["./app"]