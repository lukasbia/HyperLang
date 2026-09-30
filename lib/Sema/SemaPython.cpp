#include "hyper/Sema/Sema.h"
#include <string_view>

namespace hyper::sema::python {

bool isDynamicBuiltin(std::string_view name) {
    return name == "int" || name == "float" || name == "str" ||
           name == "bool" || name == "bytes" || name == "list" ||
           name == "dict" || name == "set" || name == "tuple";
}

bool isScopeKeyword(std::string_view name) {
    return name == "global" || name == "nonlocal" || name == "lambda" ||
           name == "yield" || name == "async" || name == "await";
}

} // namespace hyper::sema::python
