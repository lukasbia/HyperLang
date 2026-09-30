#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace hyper::llcvm::optimization {

struct OptimizationPass {
    std::string name;
    bool enabled = true;
};

class Pipeline {
public:
    void add(OptimizationPass pass);
    std::size_t size() const noexcept;
    const std::vector<OptimizationPass>& passes() const noexcept;
private:
    std::vector<OptimizationPass> passes_;
};

} // namespace hyper::llcvm::optimization
