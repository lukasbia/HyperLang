#include <string>
#include <utility>
#include <vector>

namespace hyper::ast {

struct Type {
    virtual ~Type() = default;
};

struct NamedType : Type {
    std::string name;
};

struct OptionalType : Type {
    Type* wrapped = nullptr;
};

struct ArrayType : Type {
    Type* element = nullptr;
};

struct DictionaryType : Type {
    Type* key = nullptr;
    Type* value = nullptr;
};

struct TupleType : Type {
    std::vector<Type*> elements;
};

struct FunctionType : Type {
    std::vector<Type*> parameters;
    Type* result = nullptr;
    bool throwing = false;
};

struct GenericType : Type {
    Type* base = nullptr;
    std::vector<Type*> arguments;
};

struct MetatypeType : Type {
    Type* instance = nullptr;
};

struct ExistentialType : Type {
    std::vector<Type*> constraints;
};

bool isNamed(const Type* type) {
    return dynamic_cast<const NamedType*>(type) != nullptr;
}

bool isOptional(const Type* type) {
    return dynamic_cast<const OptionalType*>(type) != nullptr;
}

bool isFunction(const Type* type) {
    return dynamic_cast<const FunctionType*>(type) != nullptr;
}

bool isGeneric(const Type* type) {
    return dynamic_cast<const GenericType*>(type) != nullptr;
}

} // namespace hyper::ast
