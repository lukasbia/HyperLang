//===--- ToolChains.cpp - HyperLang Tool Chain Selection ------------------===//
#include "ToolChains.h"
namespace hyperlang::driver {
std::unique_ptr<ToolChain> createToolChain(TargetInfo target) {
  if (target.platform == "darwin" || target.platform == "macos")
    return std::make_unique<DarwinToolChain>(std::move(target));
  if (target.platform == "windows")
    return std::make_unique<WindowsToolChain>(std::move(target));
  if (target.architecture == "wasm32" || target.architecture == "wasm64")
    return std::make_unique<WebAssemblyToolChain>(std::move(target));
  return std::make_unique<UnixToolChain>(std::move(target));
}
}
