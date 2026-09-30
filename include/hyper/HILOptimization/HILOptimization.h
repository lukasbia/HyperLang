#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace hyper::hil::optimization {

struct Pass {
    std::string name;
    bool enabled = true;
};

class Pipeline {
public:
    Pipeline();
    void addPass(Pass pass);
    std::size_t size() const noexcept;
    const std::vector<Pass>& passes() const noexcept;

private:
    std::vector<Pass> passes_;
};

} // namespace hyper::hil::optimization
