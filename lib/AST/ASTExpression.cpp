#include <string>
#include <utility>
#include <vector>

namespace hyper::ast {

struct Expression {
    virtual ~Expression() = default;
};

struct IdentifierExpression : Expression {
    std::string name;
};

struct LiteralExpression : Expression {
    enum class Kind {
        Integer,
        Float,
        String,
        Boolean,
        Null
    };
    Kind kind = Kind::Null;
    std::string value;
};

struct UnaryExpression : Expression {
    std::string operation;
    Expression* operand = nullptr;
};

struct BinaryExpression : Expression {
    Expression* left = nullptr;
    std::string operation;
    Expression* right = nullptr;
};

struct CallExpression : Expression {
    Expression* callee = nullptr;
    std::vector<Expression*> arguments;
};

struct MemberExpression : Expression {
    Expression* base = nullptr;
    std::string member;
};

struct SubscriptExpression : Expression {
    Expression* base = nullptr;
    std::vector<Expression*> indices;
};

struct AssignmentExpression : Expression {
    Expression* destination = nullptr;
    Expression* value = nullptr;
};

bool isLiteral(const Expression* expression) {
    return dynamic_cast<const LiteralExpression*>(expression) != nullptr;
}

bool isCall(const Expression* expression) {
    return dynamic_cast<const CallExpression*>(expression) != nullptr;
}

bool isBinary(const Expression* expression) {
    return dynamic_cast<const BinaryExpression*>(expression) != nullptr;
}

bool isMember(const Expression* expression) {
    return dynamic_cast<const MemberExpression*>(expression) != nullptr;
}

bool isSubscript(const Expression* expression) {
    return dynamic_cast<const SubscriptExpression*>(expression) != nullptr;
}

} // namespace hyper::ast
