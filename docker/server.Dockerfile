FROM debian:bookworm-slim AS build

WORKDIR /app
COPY . .

RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ cmake make pkg-config \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

RUN cmake -S server -B build/server -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build/server --target r-type_server -j$(nproc)

FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
    libstdc++6 \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /usr/local/bin

COPY --from=build /app/build/server/r-type_server .

EXPOSE 4242/udp
CMD ["./r-type_server"]
