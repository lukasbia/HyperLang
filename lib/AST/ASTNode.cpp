#include <string>
#include <string_view>

namespace hyper::ast {

enum class NodeKind {
    TranslationUnit,
    Declaration,
    Expression,
    Statement,
    Type,
    Pattern,
    Attribute,
    Modifier,
    Unknown
};

const char* nodeKindName(NodeKind kind) noexcept {
    switch (kind) {
    case NodeKind::TranslationUnit:
        return "TranslationUnit";
    case NodeKind::Declaration:
        return "Declaration";
    case NodeKind::Expression:
        return "Expression";
    case NodeKind::Statement:
        return "Statement";
    case NodeKind::Type:
        return "Type";
    case NodeKind::Pattern:
        return "Pattern";
    case NodeKind::Attribute:
        return "Attribute";
    case NodeKind::Modifier:
        return "Modifier";
    case NodeKind::Unknown:
        return "Unknown";
    }

    return "Unknown";
}

std::string_view nodeKindNameView(NodeKind kind) noexcept {
    return nodeKindName(kind);
}

bool isDeclaration(NodeKind kind) noexcept {
    return kind == NodeKind::Declaration;
}

bool isExpression(NodeKind kind) noexcept {
    return kind == NodeKind::Expression;
}

bool isStatement(NodeKind kind) noexcept {
    return kind == NodeKind::Statement;
}

bool isType(NodeKind kind) noexcept {
    return kind == NodeKind::Type;
}

bool isPattern(NodeKind kind) noexcept {
    return kind == NodeKind::Pattern;
}

} // namespace hyper::ast
