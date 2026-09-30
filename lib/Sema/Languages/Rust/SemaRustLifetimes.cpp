#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::rust { bool isLifetime(std::string_view n) { return !n.empty() && n.front()=='\''; } }
