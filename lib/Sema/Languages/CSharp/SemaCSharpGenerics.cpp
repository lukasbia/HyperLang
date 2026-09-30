#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::csharp { bool isGenericKeyword(std::string_view n) { return n=="where"||n=="in"||n=="out"||n=="class"; } }
