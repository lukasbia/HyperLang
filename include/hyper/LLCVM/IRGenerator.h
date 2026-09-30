#pragma once

#include "hyper/LLCVM/LLCVM.h"
#include <string_view>

namespace hyper::llcvm {

class IRGenerator {
public:
    CompiledLine generateLine(std::string_view source, std::size_t line,
                              std::string_view file = {}) const;
};

} // namespace hyper::llcvm
