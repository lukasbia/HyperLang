#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::sgl { bool isTypeFeature(std::string_view n) { return n=="int"||n=="float"||n=="string"||n=="bool"||n=="vector"; } }
