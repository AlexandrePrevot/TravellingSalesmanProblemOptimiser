# multi stage builds to separate building dependencies and buildint the source files
# stage 1 => install everything needed to compile
# stage 2 => build the source code so that when modified we rebuild this image
# without passing through stage 1
# stage 3 => just a minimal runtime image

# -------- stage 1 : install dependencies --------
FROM ubuntu:24.04 AS tsp_deps_builder

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    curl \
    autoconf \
    libtool \
    pkg-config \
    unzip \
    libssl-dev \
    zlib1g-dev \
    libabsl-dev \
    libc-ares-dev \
    libre2-dev

ENV GRPC_RELEASE_TAG=v1.73.0

# follow https://grpc.io/docs/languages/cpp/quickstart/#install-grpc
WORKDIR /opt
RUN git clone --recurse-submodules -b ${GRPC_RELEASE_TAG} --depth 1 --shallow-submodules https://github.com/grpc/grpc

WORKDIR /opt/grpc

RUN mkdir -p cmake/build && cd cmake/build && \
    cmake ../.. -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF \
    -DgRPC_INSTALL=ON \
    -DgRPC_BUILD_TESTS=OFF \
    -DCMAKE_INSTALL_PREFIX=/opt/grpc_install && \
    make -j 3 && make install


# -------- stage 2 : build backend source files --------

FROM tsp_deps_builder AS tsp_builder

COPY backend /app/backend
COPY messages /app/messages

WORKDIR /app

# to avoid version mismatch use grpc protoc 
RUN /opt/grpc_install/bin/protoc --proto_path=messages/ --cpp_out=backend/generated --grpc_out=backend/generated/ --plugin=protoc-gen-grpc=/opt/grpc_install/bin/grpc_cpp_plugin messages/request.proto

WORKDIR /app/build

RUN cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="/opt/grpc_install" -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ../backend/ && \
    make -j 3

# -------- stage 3 : minimum runtime image --------

FROM ubuntu:24.04

COPY --from=tsp_builder /app/build/TravellingSalesmanProblemOptimiser /usr/local/bin/tsp_server

# might use entrypoint to start application right away
