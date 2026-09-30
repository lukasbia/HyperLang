#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::shell { bool isExpansionFeature(std::string_view n) { return n=="parameter"||n=="command"||n=="arithmetic"||n=="glob"; } }
