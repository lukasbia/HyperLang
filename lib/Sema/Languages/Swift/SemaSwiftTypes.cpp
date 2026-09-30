#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::swift { bool isType(std::string_view n){return n=="Int"||n=="UInt"||n=="Bool"||n=="Float"||n=="Double"||n=="String"||n=="Character"||n=="Any"||n=="Never";} } // namespace hyper::sema::swift
