#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::python { bool isBuiltinType(std::string_view n){return n=="int"||n=="float"||n=="str"||n=="bool"||n=="bytes"||n=="list"||n=="dict"||n=="set"||n=="tuple"||n=="object";} } // namespace hyper::sema::python
