#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::php { bool isTraitFeature(std::string_view n) { return n=="trait"||n=="use"||n=="interface"||n=="namespace"; } }
