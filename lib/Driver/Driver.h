#ifndef HYPERLANG_DRIVER_DRIVER_H
#define HYPERLANG_DRIVER_DRIVER_H
#include "hyperlang/Driver/Compilation.h"
#include <memory>
#include <string>
#include <vector>
namespace hyperlang::driver {
class ToolChain;
class Driver {
public:
  Driver();
  explicit Driver(TargetInfo target);
  void setTarget(TargetInfo target);
  const TargetInfo &target() const;
  std::unique_ptr<ToolChain> createToolChain() const;
  Compilation buildCompilation(const std::vector<std::string> &inputs) const;
private:
  TargetInfo target_;
};
}
#endif
