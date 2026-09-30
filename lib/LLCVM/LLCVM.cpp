#include "hyper/LLCVM/LLCVM.h"
#include <utility>

namespace hyper::llcvm {

Module::Module(std::string name) : name_(std::move(name)) {}

void Module::updateLine(CompiledLine line) {
    for (auto& existing : lines_) {
        if (existing.sourceLine == line.sourceLine) {
            existing = std::move(line);
            return;
        }
    }
    lines_.push_back(std::move(line));
}

const std::string& Module::name() const noexcept { return name_; }

const std::vector<CompiledLine>& Module::lines() const noexcept { return lines_; }

} // namespace hyper::llcvm
