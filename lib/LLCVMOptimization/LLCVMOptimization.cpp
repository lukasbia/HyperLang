#include "hyper/LLCVMOptimization/LLCVMOptimization.h"
#include <utility>

namespace hyper::llcvm::optimization {

void Pipeline::add(OptimizationPass pass) {
    passes_.push_back(std::move(pass));
}

std::size_t Pipeline::size() const noexcept {
    return passes_.size();
}

const std::vector<OptimizationPass>& Pipeline::passes() const noexcept {
    return passes_;
}

} // namespace hyper::llcvm::optimization
