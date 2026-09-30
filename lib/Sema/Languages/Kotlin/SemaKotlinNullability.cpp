#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::kotlin { bool isNullableType(std::string_view n) { return !n.empty() && n.back()=='?'; } }
