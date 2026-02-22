#!/usr/bin/env bash
# NOTE You may need to run: sudo apt install ninja-build if you haven't alread if you haven't alreadyy

set -euo pipefail

cmake -B build -S . -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build

