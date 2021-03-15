#!/usr/bin/env bash

set -e

(
    mkdir -p release
    cd release
    conan install .. --build=missing
    cmake .. -DCMAKE_CXX_FLAGS="-Werror -Wall -Wextra -O2" -DCMAKE_BUILD_TYPE=Release
    make -j
)

if [ ! -e static ]; then
    git clone https://gitlab.com/epi-codes/Epitech-2023-Timeline.git static
fi

ln -sf "$(pwd)/static/data/timeline-2025.json" ./static/data.json

xdg-open http://localhost:8080 || echo Open http://localhost:8080 in your navigator

echo Starting zia
./release/bin/zia config.yml release/lib
