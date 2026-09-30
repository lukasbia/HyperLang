#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::swift { bool isOwnershipModifier(std::string_view n) { return n=="borrowing"||n=="consuming"||n=="inout"; } }
