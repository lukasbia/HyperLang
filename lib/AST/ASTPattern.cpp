#include <string>
#include <vector>

namespace hyper::ast {

struct Pattern {
    virtual ~Pattern() = default;
};

struct IdentifierPattern : Pattern {
    std::string name;
};

struct WildcardPattern : Pattern {
};

struct TuplePattern : Pattern {
    std::vector<Pattern*> elements;
};

struct ValueBindingPattern : Pattern {
    bool mutableBinding = false;
    Pattern* pattern = nullptr;
};

struct EnumPattern : Pattern {
    std::string caseName;
    std::vector<Pattern*> associatedValues;
};

struct OptionalPattern : Pattern {
    Pattern* wrapped = nullptr;
};

struct TypePattern : Pattern {
    Pattern* pattern = nullptr;
    void* type = nullptr;
};

bool isWildcard(const Pattern* pattern) {
    return dynamic_cast<const WildcardPattern*>(pattern) != nullptr;
}

bool isIdentifier(const Pattern* pattern) {
    return dynamic_cast<const IdentifierPattern*>(pattern) != nullptr;
}

bool isTuple(const Pattern* pattern) {
    return dynamic_cast<const TuplePattern*>(pattern) != nullptr;
}

bool isBinding(const Pattern* pattern) {
    return dynamic_cast<const ValueBindingPattern*>(pattern) != nullptr;
}

} // namespace hyper::ast
