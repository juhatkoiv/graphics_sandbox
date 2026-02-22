#!/usr/bin/env bash
set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
VCPKG_DIR="$REPO_ROOT/third_party/vcpkg"

echo "==> Initialising vcpkg submodule..."
git -C "$REPO_ROOT" submodule update --init --recursive third_party/vcpkg

echo "==> Bootstrapping vcpkg..."
"$VCPKG_DIR/bootstrap-vcpkg.sh" -disableMetrics

echo "==> Installing dependencies (vcpkg.json manifest)..."
"$VCPKG_DIR/vcpkg" install \
    --triplet x64-linux \
    --x-manifest-root="$REPO_ROOT" \
    --x-install-root="$REPO_ROOT/third_party/vcpkg_installed"

echo ""
echo "Done. Now configure with CMake:"
echo "  cmake -B build -S . -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -G Ninja"
echo "  cmake --build build"
