//===--- UnixToolChains.cpp - Unix Tool Chains ----------------------------===//
#include "hyperlang/Driver/ToolChains.h"
namespace hyperlang::driver {
UnixToolChain::UnixToolChain(TargetInfo target) : ToolChain(std::move(target)) {}
std::string UnixToolChain::platformName() const { return "unix"; }
}
