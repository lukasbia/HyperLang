#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::go { bool supportsGenericSyntax(std::string_view n) { return n=="any"||n=="comparable"; } }
