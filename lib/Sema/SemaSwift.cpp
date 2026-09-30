#include "hyper/Sema/Sema.h"
#include <string_view>

namespace hyper::sema::swift {

bool supportsOwnershipKeyword(std::string_view name) {
    return name == "borrowing" || name == "consuming" || name == "inout";
}

bool supportsConcurrencyKeyword(std::string_view name) {
    return name == "async" || name == "await" || name == "actor" || name == "Sendable";
}

} // namespace hyper::sema::swift
