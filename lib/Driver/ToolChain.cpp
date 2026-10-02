//===--- ToolChain.cpp - HyperLang Tool Chain -----------------------------===//
#include "hyperlang/Driver/ToolChains.h"
namespace hyperlang::driver {
ToolChain::ToolChain(TargetInfo target) : target_(std::move(target)) {}
const TargetInfo &ToolChain::target() const { return target_; }
std::string ToolChain::compiler() const { return "clang++"; }
std::string ToolChain::assembler() const { return "as"; }
std::string ToolChain::linker() const { return "ld"; }
}
