#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::java { bool isPrimitive(std::string_view n){return n=="byte"||n=="short"||n=="int"||n=="long"||n=="float"||n=="double"||n=="char"||n=="boolean"||n=="void";} } // namespace hyper::sema::java
