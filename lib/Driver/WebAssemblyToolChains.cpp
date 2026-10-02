//===--- WebAssemblyToolChains.cpp - WebAssembly Tool Chain ---------------===//
#include "hyperlang/Driver/ToolChains.h"
namespace hyperlang::driver {
WebAssemblyToolChain::WebAssemblyToolChain(TargetInfo target) : ToolChain(std::move(target)) {}
std::string WebAssemblyToolChain::compiler() const { return "clang"; }
std::string WebAssemblyToolChain::assembler() const { return "wasm-ld"; }
std::string WebAssemblyToolChain::linker() const { return "wasm-ld"; }
std::string WebAssemblyToolChain::platformName() const { return "webassembly"; }
}
