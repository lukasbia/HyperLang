#pragma once
#include "hyperlang/HIL/HIL.h"
#include <functional>
#include <string>
namespace hyperlang::lsc {
struct CompilationSnapshot { std::size_t revision=0; hil::Module module; };
class LiveCompiler {
public:
 using UpdateCallback=std::function<void(const CompilationSnapshot&)>;
 void synchronize(const hil::Module& module);
 void setUpdateCallback(UpdateCallback callback);
 const CompilationSnapshot& snapshot() const;
private: CompilationSnapshot snapshot_{}; UpdateCallback callback_;
};
}
