//===--- WindowsToolChains.cpp - Windows Tool Chain ----------------------===//
#include "ToolChains.h"
namespace hyperlang::driver {
WindowsToolChain::WindowsToolChain(TargetInfo target) : ToolChain(std::move(target)) {}
std::string WindowsToolChain::compiler() const { return "clang++"; }
std::string WindowsToolChain::assembler() const { return "llvm-mc"; }
std::string WindowsToolChain::linker() const { return "lld-link"; }
std::string WindowsToolChain::platformName() const { return "windows"; }
}
