#include <string>
#include <vector>

namespace hyper::ast {

struct OwnershipModifier {
    enum class Kind {
        Borrowed,
        Consuming,
        Inout,
        Owned,
        Shared
    };

    Kind kind = Kind::Shared;
    std::string name;
};

struct OwnershipExpression {
    OwnershipModifier::Kind kind = OwnershipModifier::Kind::Shared;
    void* value = nullptr;
};

bool isBorrowed(const OwnershipModifier& modifier) {
    return modifier.kind == OwnershipModifier::Kind::Borrowed;
}

bool isConsuming(const OwnershipModifier& modifier) {
    return modifier.kind == OwnershipModifier::Kind::Consuming;
}

bool isInout(const OwnershipModifier& modifier) {
    return modifier.kind == OwnershipModifier::Kind::Inout;
}

} // namespace hyper::ast
