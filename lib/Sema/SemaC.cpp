#include "hyper/Sema/Sema.h"
#include <string_view>

namespace hyper::sema::c {

bool isPrimitiveType(std::string_view name) {
    return name == "void" || name == "char" || name == "short" ||
           name == "int" || name == "long" || name == "float" ||
           name == "double" || name == "_Bool";
}

bool isStorageSpecifier(std::string_view name) {
    return name == "auto" || name == "extern" || name == "static" ||
           name == "register" || name == "typedef";
}

} // namespace hyper::sema::c
