#pragma once
#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include "hyperlang/HIL/HIL.h"
namespace hyperlang::mcm {
struct ObjectHeader { std::size_t strongReferences=1; std::size_t weakReferences=0; };
class ManagedObject { public: virtual ~ManagedObject()=default; ObjectHeader header; };
template<class T, class... Args> std::shared_ptr<T> makeManaged(Args&&... args) { return std::make_shared<T>(std::forward<Args>(args)...); }
class Manager { public: void retain(ManagedObject&); void release(ManagedObject&); };
void insertOwnershipOperations(hil::Module& module);
}
