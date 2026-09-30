#include "hyperlang/MCM/MCM.h"
#include <stdexcept>
#include <utility>

namespace hyperlang::mcm {

ObjectID Runtime::allocate(std::string typeName, Destructor destructor) {
    const ObjectID id = nextID_++;
    objects_.emplace(id, ManagedObject{id, {}, std::move(typeName), std::move(destructor)});
    ++statistics_.allocations;
    return id;
}

void Runtime::retain(ObjectID id) {
    auto it = objects_.find(id);
    if (it == objects_.end()) throw std::runtime_error("MCM retain of invalid object");
    ++it->second.header.strongReferences;
    ++statistics_.retains;
}

void Runtime::release(ObjectID id) {
    auto it = objects_.find(id);
    if (it == objects_.end()) return;
    if (it->second.header.strongReferences == 0) return;
    --it->second.header.strongReferences;
    ++statistics_.releases;
    if (it->second.header.strongReferences == 0) {
        deferredReleases_.push_back(id);
        ++statistics_.deferredReleases;
    }
}

void Runtime::weakRetain(ObjectID id) {
    auto it = objects_.find(id);
    if (it != objects_.end()) ++it->second.header.weakReferences;
}

void Runtime::weakRelease(ObjectID id) {
    auto it = objects_.find(id);
    if (it != objects_.end() && it->second.header.weakReferences != 0) --it->second.header.weakReferences;
}

void Runtime::drain() {
    for (const ObjectID id : deferredReleases_) {
        auto it = objects_.find(id);
        if (it != objects_.end() && it->second.header.strongReferences == 0) destroy(id);
    }
    deferredReleases_.clear();
}

const ManagedObject* Runtime::lookup(ObjectID id) const {
    const auto it = objects_.find(id);
    return it == objects_.end() ? nullptr : &it->second;
}

const Statistics& Runtime::statistics() const noexcept { return statistics_; }
std::size_t Runtime::liveObjectCount() const noexcept { return objects_.size(); }

void Runtime::destroy(ObjectID id) {
    auto it = objects_.find(id);
    if (it == objects_.end()) return;
    if (it->second.destructor) it->second.destructor(id);
    objects_.erase(it);
    ++statistics_.deallocations;
}

StrongReference::StrongReference(Runtime& runtime, ObjectID id) : runtime_(&runtime), id_(id) { runtime_->retain(id_); }
StrongReference::StrongReference(const StrongReference& other) : runtime_(other.runtime_), id_(other.id_) { if (runtime_) runtime_->retain(id_); }
StrongReference::StrongReference(StrongReference&& other) noexcept : runtime_(other.runtime_), id_(other.id_) { other.runtime_ = nullptr; other.id_ = 0; }
StrongReference& StrongReference::operator=(const StrongReference& other) { if (this != &other) { if (runtime_) runtime_->release(id_); runtime_ = other.runtime_; id_ = other.id_; if (runtime_) runtime_->retain(id_); } return *this; }
StrongReference& StrongReference::operator=(StrongReference&& other) noexcept { if (this != &other) { if (runtime_) runtime_->release(id_); runtime_ = other.runtime_; id_ = other.id_; other.runtime_ = nullptr; other.id_ = 0; } return *this; }
StrongReference::~StrongReference() { if (runtime_) runtime_->release(id_); }
ObjectID StrongReference::id() const noexcept { return id_; }
StrongReference::operator bool() const noexcept { return runtime_ != nullptr && id_ != 0; }

} // namespace hyperlang::mcm
