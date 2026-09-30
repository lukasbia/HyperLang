#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::rust { bool isOwnership(std::string_view n){return n=="move"||n=="borrow"||n=="ref"||n=="mut"||n=="const";} } // namespace hyper::sema::rust
