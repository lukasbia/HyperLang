#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::objectivec { bool isMessagingFeature(std::string_view n) { return n=="selector"||n=="message"||n=="protocol"||n=="category"; } }
