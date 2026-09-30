#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::ruby { bool isBlockFeature(std::string_view n) { return n=="yield"||n=="proc"||n=="lambda"||n=="block"; } }
