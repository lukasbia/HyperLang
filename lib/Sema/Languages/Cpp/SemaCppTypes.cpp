#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::cpp { bool isBuiltinType(std::string_view n){return n=="void"||n=="bool"||n=="char"||n=="short"||n=="int"||n=="long"||n=="float"||n=="double"||n=="auto";} } // namespace hyper::sema::cpp
