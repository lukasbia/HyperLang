#pragma once
#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace hyperlang::mcm {

// Ownership modes understood by the compiler. Automatic is the normal mode.
enum class Ownership : std::uint8_t { Automatic, Strong, Weak, Borrowed, Owned };

struct ObjectHeader {
    std::uint32_t strongReferences = 1;
    std::uint32_t weakReferences = 0;
    std::uint32_t flags = 0;
};

using ObjectID = std::uint64_t;
using Destructor = std::function<void(ObjectID)>;

struct ManagedObject {
    ObjectID id = 0;
    ObjectHeader header;
    std::string typeName;
    Destructor destructor;
};

struct Statistics {
    std::size_t allocations = 0;
    std::size_t deallocations = 0;
    std::size_t retains = 0;
    std::size_t releases = 0;
    std::size_t deferredReleases = 0;
};

class Runtime {
public:
    ObjectID allocate(std::string typeName, Destructor destructor = {});
    void retain(ObjectID id);
    void release(ObjectID id);
    void weakRetain(ObjectID id);
    void weakRelease(ObjectID id);
    void drain();
    const ManagedObject* lookup(ObjectID id) const;
    const Statistics& statistics() const noexcept;
    std::size_t liveObjectCount() const noexcept;

private:
    ObjectID nextID_ = 1;
    std::unordered_map<ObjectID, ManagedObject> objects_;
    std::vector<ObjectID> deferredReleases_;
    Statistics statistics_;
    void destroy(ObjectID id);
};

class StrongReference {
public:
    StrongReference() = default;
    StrongReference(Runtime& runtime, ObjectID id);
    StrongReference(const StrongReference& other);
    StrongReference(StrongReference&& other) noexcept;
    StrongReference& operator=(const StrongReference& other);
    StrongReference& operator=(StrongReference&& other) noexcept;
    ~StrongReference();
    ObjectID id() const noexcept;
    explicit operator bool() const noexcept;
private:
    Runtime* runtime_ = nullptr;
    ObjectID id_ = 0;
};

} // namespace hyperlang::mcm
