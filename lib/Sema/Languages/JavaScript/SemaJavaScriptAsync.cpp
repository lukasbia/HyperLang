#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::javascript { bool isAsyncFeature(std::string_view n) { return n=="async"||n=="await"||n=="Promise"||n=="yield"; } }
