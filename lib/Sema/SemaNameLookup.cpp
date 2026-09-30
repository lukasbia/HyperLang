#include "hyper/Sema/Sema.h"
#include <string_view>

namespace hyper::sema {

bool canResolveName(const Sema& sema, std::string_view name) {
    return sema.lookup(name) != nullptr;
}

} // namespace hyper::sema
