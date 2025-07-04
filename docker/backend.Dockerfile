FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    curl \
    autoconf \
    libtool \
    pkg-config \
    unzip


# follow https://grpc.io/docs/languages/cpp/quickstart/#install-grpc
WORKDIR /opt
RUN git clone --recurse-submodules -b v1.73.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc

