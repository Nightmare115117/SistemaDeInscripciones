FROM ubuntu:24.04

RUN apt-get update && apt-get install -y \
    build-essential cmake git \
    libboost-all-dev libssl-dev libasio-dev libpq-dev \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

RUN git clone --branch 8.0.1 https://github.com/jtv/libpqxx.git /tmp/libpqxx && \
    cd /tmp/libpqxx && \
    cmake -B build -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build -j$(nproc) && \
    cmake --install build && \
    rm -rf /tmp/libpqxx

ENV PKG_CONFIG_PATH="/usr/local/lib/pkgconfig:${PKG_CONFIG_PATH}"
ENV LD_LIBRARY_PATH="/usr/local/lib:${LD_LIBRARY_PATH}"
RUN ldconfig

WORKDIR /app
COPY . .

RUN rm -rf build && mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j$(nproc)

EXPOSE 8080
CMD ["./build/mi_api"]