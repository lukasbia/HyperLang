#include "hyper/Sema/Sema.h"
#include <string_view>

namespace hyper::sema::cpp {

bool isTypeKeyword(std::string_view name) {
    return name == "auto" || name == "void" || name == "bool" ||
           name == "char" || name == "short" || name == "int" ||
           name == "long" || name == "float" || name == "double";
}

bool isClassKeyword(std::string_view name) {
    return name == "class" || name == "struct" || name == "union" ||
           name == "enum" || name == "namespace";
}

} // namespace hyper::sema::cpp
