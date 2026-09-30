#include "hyper/LLCVM/LLCVM.h"
#include "hyper/LLCVM/IRGenerator.h"

namespace hyper::llcvm {

CompiledLine compileLine(std::string_view source, std::size_t line, std::string_view file) {
    IRGenerator generator;
    return generator.generateLine(source, line, file);
}

} // namespace hyper::llcvm
