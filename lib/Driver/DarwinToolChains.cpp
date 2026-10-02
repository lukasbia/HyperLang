//===--- DarwinToolChains.cpp - Darwin Tool Chain -------------------------===//
#include "ToolChains.h"
namespace hyperlang::driver {
DarwinToolChain::DarwinToolChain(TargetInfo target) : ToolChain(std::move(target)) {}
std::string DarwinToolChain::linker() const { return "ld"; }
std::string DarwinToolChain::assembler() const { return "as"; }
std::string DarwinToolChain::platformName() const { return "darwin"; }
}
