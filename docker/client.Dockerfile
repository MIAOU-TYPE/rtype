FROM debian:bookworm-slim AS build

RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ cmake make pkg-config libsfml-dev \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN cmake -S client -B build/client -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build/client --target r-type_client -j$(nproc)


FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
    libstdc++6 libsfml-dev \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /usr/local/bin
COPY --from=build /app/build/client/r-type_client .

CMD ["./r-type_client"]
