//===--- Driver.cpp - HyperLang Compiler Driver ---------------------------===//
#include "hyperlang/Driver/Driver.h"
#include "hyperlang/Driver/ToolChains.h"
#include <utility>
namespace hyperlang::driver {
Driver::Driver() = default;
Driver::Driver(TargetInfo target) : target_(std::move(target)) {}
void Driver::setTarget(TargetInfo target) { target_ = std::move(target); }
const TargetInfo &Driver::target() const { return target_; }

std::unique_ptr<ToolChain> Driver::createToolChain() const {
  if (target_.platform == "darwin" || target_.platform == "macos")
    return std::make_unique<DarwinToolChain>(target_);
  if (target_.platform == "windows")
    return std::make_unique<WindowsToolChain>(target_);
  if (target_.architecture == "wasm32" || target_.architecture == "wasm64")
    return std::make_unique<WebAssemblyToolChain>(target_);
  return std::make_unique<UnixToolChain>(target_);
}

Compilation Driver::buildCompilation(const std::vector<std::string> &inputs) const {
  Compilation compilation;
  auto toolChain = createToolChain();
  for (const auto &input : inputs) {
    Job parse{ActionKind::Parse, "hyperlang-parser", {input}, input + ".parsed"};
    compilation.addJob(std::move(parse));
    Job sema{ActionKind::Sema, "hyperlang-sema", {input + ".parsed"}, input + ".checked"};
    compilation.addJob(std::move(sema));
    Job hil{ActionKind::HILGen, "hyperlang-hilgen", {input + ".checked"}, input + ".hil"};
    compilation.addJob(std::move(hil));
    Job opt{ActionKind::HILOptimize, "hyperlang-hil-opt", {input + ".hil"}, input + ".optimized.hil"};
    compilation.addJob(std::move(opt));
    Job lsc{ActionKind::LSCGen, "hyperlang-lscgen", {input + ".optimized.hil"}, input + ".lsc"};
    compilation.addJob(std::move(lsc));
    Job codegen{ActionKind::CodeGen, toolChain->compiler(), {input + ".lsc"}, input + ".o"};
    compilation.addJob(std::move(codegen));
    Job assemble{ActionKind::Assemble, toolChain->assembler(), {input + ".o"}, input + ".obj"};
    compilation.addJob(std::move(assemble));
  }
  if (!inputs.empty()) {
    std::vector<std::string> objects;
    for (const auto &input : inputs) objects.push_back(input + ".obj");
    compilation.addJob(Job{ActionKind::Link, toolChain->linker(), objects, "a.out"});
  }
  return compilation;
}
}
