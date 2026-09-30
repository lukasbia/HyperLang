#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::typescript { bool isPrimitive(std::string_view n){return n=="any"||n=="unknown"||n=="never"||n=="void"||n=="undefined"||n=="null"||n=="boolean"||n=="number"||n=="bigint"||n=="string"||n=="symbol";} } // namespace hyper::sema::typescript
