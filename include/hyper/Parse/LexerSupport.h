#pragma once

#include <cstddef>
#include <string>
#include <string_view>

namespace hyper {

struct SourceLocation;
struct Token;

enum class LexerDiagnosticSeverity {
    Note,
    Warning,
    Error
};

struct LexerDiagnostic {
    LexerDiagnosticSeverity severity = LexerDiagnosticSeverity::Error;
    SourceLocation* location = nullptr;
    std::string message;
    std::string replacement;
    std::string category;
    std::size_t offset = 0;
    std::size_t length = 0;
};

class LexerSupport {
public:
    static bool isIdentifierStart(std::string_view text) noexcept;
    static bool isIdentifierContinue(std::string_view text) noexcept;
    static bool isOperator(std::string_view text) noexcept;
    static bool isWhitespace(std::string_view text) noexcept;
    static bool isLineBreak(std::string_view text) noexcept;
    static bool isComment(std::string_view text) noexcept;
    static bool isDirective(std::string_view text) noexcept;
    static bool isEditorPlaceholder(std::string_view text) noexcept;
    static bool isDocumentationComment(std::string_view text) noexcept;
    static bool isCompilerDirective(std::string_view text) noexcept;
    static std::size_t indentationWidth(std::string_view text) noexcept;
    static std::string normalizeIdentifier(std::string_view text);
    static std::string unescapeIdentifier(std::string_view text);
    static std::string stripCommentMarkers(std::string_view text);
};

} // namespace hyper
