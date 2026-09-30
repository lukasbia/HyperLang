#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::java { bool supportsGenerics(std::string_view name) { return name == "List" || name == "Map" || name == "Set" || name == "Optional"; } }
