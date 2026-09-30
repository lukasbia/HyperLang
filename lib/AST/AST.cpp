#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace hyper::ast {

namespace {

struct NodeStorage {
    std::vector<std::unique_ptr<class Node>> nodes;
};

NodeStorage& storage() {
    static NodeStorage value;
    return value;
}

}

class Node {
public:
    virtual ~Node() = default;

    virtual std::string kindName() const = 0;

    virtual std::string dump() const {
        return kindName();
    }
};

class TranslationUnit final : public Node {
public:
    std::vector<std::unique_ptr<Node>> declarations;

    std::string kindName() const override {
        return "TranslationUnit";
    }

    std::string dump() const override {
        std::string result = kindName();
        for (const auto& declaration : declarations) {
            if (declaration) {
                result += "\n  ";
                result += declaration->dump();
            }
        }
        return result;
    }
};

class Identifier final : public Node {
public:
    explicit Identifier(std::string value)
        : value_(std::move(value)) {
    }

    std::string kindName() const override {
        return "Identifier";
    }

    std::string dump() const override {
        return kindName() + "(" + value_ + ")";
    }

    const std::string& value() const noexcept {
        return value_;
    }

private:
    std::string value_;
};

class Literal final : public Node {
public:
    enum class Kind {
        Integer,
        Floating,
        String,
        Boolean,
        Null
    };

    Literal(
        Kind kind,
        std::string value
    )
        : kind_(kind),
          value_(std::move(value)) {
    }

    std::string kindName() const override {
        return "Literal";
    }

    std::string dump() const override {
        return kindName() + "(" + value_ + ")";
    }

private:
    Kind kind_;
    std::string value_;
};

class Declaration : public Node {
public:
    explicit Declaration(std::string name)
        : name_(std::move(name)) {
    }

    const std::string& name() const noexcept {
        return name_;
    }

protected:
    std::string name_;
};

class FunctionDeclaration final : public Declaration {
public:
    explicit FunctionDeclaration(std::string name)
        : Declaration(std::move(name)) {
    }

    std::string kindName() const override {
        return "FunctionDeclaration";
    }

    std::string dump() const override {
        return kindName() + "(" + name_ + ")";
    }
};

class VariableDeclaration final : public Declaration {
public:
    VariableDeclaration(
        std::string name,
        bool mutableValue
    )
        : Declaration(std::move(name)),
          mutableValue_(mutableValue) {
    }

    std::string kindName() const override {
        return "VariableDeclaration";
    }

    std::string dump() const override {
        return kindName() + "(" + name_ + ")";
    }

    bool isMutable() const noexcept {
        return mutableValue_;
    }

private:
    bool mutableValue_;
};

class Expression : public Node {
public:
    ~Expression() override = default;
};

class BinaryExpression final : public Expression {
public:
    BinaryExpression(
        std::unique_ptr<Expression> left,
        std::string operation,
        std::unique_ptr<Expression> right
    )
        : left_(std::move(left)),
          operation_(std::move(operation)),
          right_(std::move(right)) {
    }

    std::string kindName() const override {
        return "BinaryExpression";
    }

    std::string dump() const override {
        std::string result = kindName();
        result += "(";
        result += operation_;
        result += ")";
        return result;
    }

private:
    std::unique_ptr<Expression> left_;
    std::string operation_;
    std::unique_ptr<Expression> right_;
};

class CallExpression final : public Expression {
public:
    explicit CallExpression(
        std::unique_ptr<Expression> callee
    )
        : callee_(std::move(callee)) {
    }

    std::string kindName() const override {
        return "CallExpression";
    }

    std::string dump() const override {
        return kindName();
    }

private:
    std::unique_ptr<Expression> callee_;
};

class Statement : public Node {
public:
    ~Statement() override = default;
};

class CompoundStatement final : public Statement {
public:
    std::vector<std::unique_ptr<Statement>> statements;

    std::string kindName() const override {
        return "CompoundStatement";
    }

    std::string dump() const override {
        std::string result = kindName();
        for (const auto& statement : statements) {
            if (statement) {
                result += "\n  ";
                result += statement->dump();
            }
        }
        return result;
    }
};

class IfStatement final : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> thenBranch;
    std::unique_ptr<Statement> elseBranch;

    std::string kindName() const override {
        return "IfStatement";
    }
};

class WhileStatement final : public Statement {
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> body;

    std::string kindName() const override {
        return "WhileStatement";
    }
};

class ReturnStatement final : public Statement {
public:
    std::unique_ptr<Expression> value;

    std::string kindName() const override {
        return "ReturnStatement";
    }
};

class TypeNode : public Node {
public:
    ~TypeNode() override = default;
};

class NamedType final : public TypeNode {
public:
    explicit NamedType(std::string name)
        : name_(std::move(name)) {
    }

    std::string kindName() const override {
        return "NamedType";
    }

    std::string dump() const override {
        return kindName() + "(" + name_ + ")";
    }

private:
    std::string name_;
};

class FunctionType final : public TypeNode {
public:
    std::vector<std::unique_ptr<TypeNode>> parameters;
    std::unique_ptr<TypeNode> result;

    std::string kindName() const override {
        return "FunctionType";
    }
};

class ArrayType final : public TypeNode {
public:
    std::unique_ptr<TypeNode> element;

    std::string kindName() const override {
        return "ArrayType";
    }
};

class OptionalType final : public TypeNode {
public:
    std::unique_ptr<TypeNode> wrapped;

    std::string kindName() const override {
        return "OptionalType";
    }
};

class GenericType final : public TypeNode {
public:
    std::unique_ptr<TypeNode> base;
    std::vector<std::unique_ptr<TypeNode>> arguments;

    std::string kindName() const override {
        return "GenericType";
    }
};

class Pattern : public Node {
public:
    ~Pattern() override = default;
};

class IdentifierPattern final : public Pattern {
public:
    explicit IdentifierPattern(std::string name)
        : name_(std::move(name)) {
    }

    std::string kindName() const override {
        return "IdentifierPattern";
    }

private:
    std::string name_;
};

class TuplePattern final : public Pattern {
public:
    std::vector<std::unique_ptr<Pattern>> elements;

    std::string kindName() const override {
        return "TuplePattern";
    }
};

class GenericParameter final : public Node {
public:
    explicit GenericParameter(std::string name)
        : name_(std::move(name)) {
    }

    std::string kindName() const override {
        return "GenericParameter";
    }

private:
    std::string name_;
};

class GenericParameterClause final : public Node {
public:
    std::vector<std::unique_ptr<GenericParameter>> parameters;

    std::string kindName() const override {
        return "GenericParameterClause";
    }
};

class Attribute final : public Node {
public:
    explicit Attribute(std::string name)
        : name_(std::move(name)) {
    }

    std::string kindName() const override {
        return "Attribute";
    }

private:
    std::string name_;
};

class Modifier final : public Node {
public:
    explicit Modifier(std::string name)
        : name_(std::move(name)) {
    }

    std::string kindName() const override {
        return "Modifier";
    }

private:
    std::string name_;
};

class ASTContext {
public:
    ASTContext() = default;

    template <typename NodeType, typename... Arguments>
    NodeType* create(
        Arguments&&... arguments
    ) {
        auto node = std::make_unique<NodeType>(
            std::forward<Arguments>(arguments)...
        );
        NodeType* result = node.get();
        owned_.push_back(std::move(node));
        return result;
    }

    void clear() {
        owned_.clear();
    }

    std::size_t size() const noexcept {
        return owned_.size();
    }

private:
    std::vector<std::unique_ptr<Node>> owned_;
};

std::string dumpNode(
    const Node& node
) {
    return node.dump();
}

std::string nodeKindName(
    const Node& node
) {
    return node.kindName();
}

bool isExpressionNode(
    const Node& node
) {
    return dynamic_cast<const Expression*>(&node) != nullptr;
}

bool isStatementNode(
    const Node& node
) {
    return dynamic_cast<const Statement*>(&node) != nullptr;
}

bool isTypeNode(
    const Node& node
) {
    return dynamic_cast<const TypeNode*>(&node) != nullptr;
}

bool isDeclarationNode(
    const Node& node
) {
    return dynamic_cast<const Declaration*>(&node) != nullptr;
}

} // namespace hyper::ast
