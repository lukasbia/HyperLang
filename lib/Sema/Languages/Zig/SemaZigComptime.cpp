#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::zig { bool isCompileTimeKeyword(std::string_view n) { return n=="comptime"||n=="inline"||n=="noinline"; } }
