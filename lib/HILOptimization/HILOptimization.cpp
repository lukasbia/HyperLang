#include "hyper/HILOptimization/HILOptimization.h"

namespace hyper::hil::optimization {

Pipeline::Pipeline() = default;

void Pipeline::addPass(Pass pass) {
    passes_.push_back(std::move(pass));
}

std::size_t Pipeline::size() const noexcept {
    return passes_.size();
}

const std::vector<Pass>& Pipeline::passes() const noexcept {
    return passes_;
}

} // namespace hyper::hil::optimization
