FROM debian:bookworm-slim

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ \
    cmake \
    git \
    make \
    pkg-config \
    libsfml-dev \
    clang \
    clang-tools \
    ccache \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

ENV CCACHE_DIR=/root/.ccache
ENV CC="ccache gcc"
ENV CXX="ccache g++"

WORKDIR /app