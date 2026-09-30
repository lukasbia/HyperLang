#include "hyper/Sema/Sema.h"
#include <string_view>

namespace hyper::sema::rust {

bool isOwnershipKeyword(std::string_view name) {
    return name == "move" || name == "ref" || name == "mut" ||
           name == "borrow" || name == "const";
}

bool isControlKeyword(std::string_view name) {
    return name == "match" || name == "loop" || name == "while" ||
           name == "for" || name == "if" || name == "else";
}

} // namespace hyper::sema::rust
