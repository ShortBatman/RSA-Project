#!/bin/bash
# Compiles rsa.cpp + ui_bridge.cpp to rsa.wasm and builds the single-file page.
# Needs wasi-sdk (https://github.com/WebAssembly/wasi-sdk). Set WASI_SDK to its folder.
set -e
WASI_SDK=${WASI_SDK:-/tmp/wasi/wasi-sdk-25.0-x86_64-linux}
cd "$(dirname "$0")"
# the wasi-sdk C++ library has no exception support, so for the web build only
# a "throw" in rsa.cpp becomes a stop (trap) that the page catches
$WASI_SDK/bin/clang++ --sysroot=$WASI_SDK/share/wasi-sysroot -std=c++17 -O2 \
  -fno-exceptions -include wasm_throw.h \
  -o rsa.wasm ../rsa.cpp ui_bridge.cpp \
  -mexec-model=reactor -Wl,--export-all -Wl,--no-entry -Wl,--strip-all
python3 build_page.py
