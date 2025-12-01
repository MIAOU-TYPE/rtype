# This Dockerfile is only for Linux testing purpose
FROM debian:bookworm-slim

ENV DEBIAN_FRONTEND=noninteractive

# ---------------------------------------------
# Dependencies
# ---------------------------------------------
RUN apt update && apt install -y \
    build-essential \
    cmake \
    git \
    wget \
    tar \
    autoconf \
    automake \
    make \
    libtool \
    pkg-config \
    python3 \
    python3-pip \
    g++ \
    gcc \
    ninja-build \
    curl \
    bash \
    zip \
    unzip \
    libx11-dev \
    libxrandr-dev \
    libxcursor-dev \
    libxi-dev \
    libudev-dev \
    libgl1-mesa-dev

# ---------------------------------------------
# Install vcpkg
# ---------------------------------------------
WORKDIR /opt
ENV VCPKG_FORCE_SYSTEM_BINARIES=1

RUN git clone https://github.com/microsoft/vcpkg.git && \
    ./vcpkg/bootstrap-vcpkg.sh

ENV VCPKG_ROOT=/opt/vcpkg
ENV PATH="${VCPKG_ROOT}:${PATH}"

# ---------------------------------------------
# Copy the project
# ---------------------------------------------
WORKDIR /app
COPY . .

# ---------------------------------------------
# Configure + Build
# ---------------------------------------------
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake

RUN cmake --build build -j$(nproc)

CMD ["/bin/bash"]
