#include <string>
#include <utility>
#include <vector>

namespace hyper::ast {

struct Statement {
    virtual ~Statement() = default;
};

struct CompoundStatement : Statement {
    std::vector<Statement*> statements;
};

struct IfStatement : Statement {
    void* condition = nullptr;
    Statement* thenBranch = nullptr;
    Statement* elseBranch = nullptr;
};

struct WhileStatement : Statement {
    void* condition = nullptr;
    Statement* body = nullptr;
};

struct ForStatement : Statement {
    void* pattern = nullptr;
    void* sequence = nullptr;
    Statement* body = nullptr;
};

struct DoStatement : Statement {
    Statement* body = nullptr;
};

struct GuardStatement : Statement {
    void* condition = nullptr;
    Statement* elseBranch = nullptr;
};

struct ReturnStatement : Statement {
    void* value = nullptr;
};

struct BreakStatement : Statement {
    std::string label;
};

struct ContinueStatement : Statement {
    std::string label;
};

struct ThrowStatement : Statement {
    void* value = nullptr;
};

struct DeferStatement : Statement {
    Statement* body = nullptr;
};

bool isCompound(const Statement* statement) {
    return dynamic_cast<const CompoundStatement*>(statement) != nullptr;
}

bool isControlFlow(const Statement* statement) {
    return dynamic_cast<const IfStatement*>(statement) != nullptr ||
           dynamic_cast<const WhileStatement*>(statement) != nullptr ||
           dynamic_cast<const ForStatement*>(statement) != nullptr ||
           dynamic_cast<const GuardStatement*>(statement) != nullptr;
}

bool isTransfer(const Statement* statement) {
    return dynamic_cast<const ReturnStatement*>(statement) != nullptr ||
           dynamic_cast<const BreakStatement*>(statement) != nullptr ||
           dynamic_cast<const ContinueStatement*>(statement) != nullptr ||
           dynamic_cast<const ThrowStatement*>(statement) != nullptr;
}

} // namespace hyper::ast
