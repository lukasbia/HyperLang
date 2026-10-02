//===--- ToolChains.h - HyperLang Driver Tool Chains ----------------------===//
#ifndef HYPERLANG_DRIVER_TOOLCHAINS_H
#define HYPERLANG_DRIVER_TOOLCHAINS_H
#include <memory>
#include <string>
#include "hyperlang/Driver/TargetInfo.h"
namespace hyperlang::driver {
class ToolChain {
public:
  explicit ToolChain(TargetInfo target);
  virtual ~ToolChain() = default;
  const TargetInfo &target() const;
  virtual std::string compiler() const;
  virtual std::string assembler() const;
  virtual std::string linker() const;
  virtual std::string platformName() const = 0;
protected:
  TargetInfo target_;
};
class UnixToolChain final : public ToolChain {
public:
  explicit UnixToolChain(TargetInfo target);
  std::string platformName() const override;
};
class DarwinToolChain final : public ToolChain {
public:
  explicit DarwinToolChain(TargetInfo target);
  std::string linker() const override;
  std::string assembler() const override;
  std::string platformName() const override;
};
class WindowsToolChain final : public ToolChain {
public:
  explicit WindowsToolChain(TargetInfo target);
  std::string compiler() const override;
  std::string assembler() const override;
  std::string linker() const override;
  std::string platformName() const override;
};
class WebAssemblyToolChain final : public ToolChain {
public:
  explicit WebAssemblyToolChain(TargetInfo target);
  std::string compiler() const override;
  std::string assembler() const override;
  std::string linker() const override;
  std::string platformName() const override;
};
std::unique_ptr<ToolChain> createToolChain(TargetInfo target);
}
#endif
