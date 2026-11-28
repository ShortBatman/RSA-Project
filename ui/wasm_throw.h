// only used for the web build (see build_wasm.sh)
// turns "throw something;" into a stop so rsa.cpp can be compiled without exceptions
#pragma once
#define throw __builtin_trap(), (void)
