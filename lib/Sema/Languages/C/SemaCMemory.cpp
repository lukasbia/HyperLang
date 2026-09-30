#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::c { bool isMemoryOperation(std::string_view n) { return n=="malloc"||n=="calloc"||n=="realloc"||n=="free"; } }
