#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::javascript { bool isPrimitive(std::string_view n){return n=="undefined"||n=="null"||n=="boolean"||n=="number"||n=="bigint"||n=="string"||n=="symbol";} } // namespace hyper::sema::javascript
