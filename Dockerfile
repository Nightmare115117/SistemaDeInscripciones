FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    build-essential cmake git \
    libboost-all-dev libpqxx-dev libssl-dev \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN rm -rf build && mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j$(nproc)

EXPOSE 8080
CMD ["./build/mi_api"]