#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::python { bool isDynamicFeature(std::string_view n) { return n=="dynamic"||n=="property"||n=="descriptor"; } }
