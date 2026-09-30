#pragma once

#include "hyper/LexerToken.h"
#include <cstddef>
#include <string>
#include <vector>

namespace hyper::parse {

enum class ParseMode {
    SourceFile,
    Declaration,
    Statement,
    Expression,
    Type,
    Pattern,
    GenericArgument,
    Directive
};

struct SourcePosition {
    std::size_t offset = 0;
    std::size_t line = 1;
    std::size_t column = 1;
};

struct SourceRange {
    SourcePosition start;
    SourcePosition end;
};

struct ParserCursor {
    std::size_t index = 0;
    std::size_t lookahead = 0;
};

class ParserCore {
public:
    ParserCore() = default;
    virtual ~ParserCore() = default;

    virtual bool isAtEnd() const = 0;
    virtual const Token& currentToken() const = 0;
    virtual const Token& lookaheadToken(std::size_t distance) const = 0;
    virtual void advanceToken() = 0;
    virtual bool consume(TokenKind kind) = 0;
    virtual bool expect(TokenKind kind) = 0;
};

} // namespace hyper::parse
