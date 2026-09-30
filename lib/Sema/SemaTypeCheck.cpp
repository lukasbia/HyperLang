#include "hyper/Sema/Sema.h"
#include <string>
#include <string_view>

namespace hyper::sema {

bool isKnownBuiltinType(std::string_view name) {
    return name == "Void" || name == "Bool" || name == "Int" ||
           name == "Float" || name == "Double" || name == "String" ||
           name == "Character" || name == "Byte" || name == "Any" ||
           name == "Never";
}

bool isAssignable(const TypeInfo& destination, const TypeInfo& source) {
    if (!destination.valid || !source.valid) {
        return false;
    }
    return destination.name == source.name || destination.name == "Any";
}

} // namespace hyper::sema
