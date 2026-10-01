#pragma once

#include "hyper/Parse/LexerToken.h"

#include <cstddef>
#include <initializer_list>

namespace hyper::parse {

enum class ParseMode {
    SourceFile, Declaration, Statement, Expression, Type,
    Pattern, GenericArgument, Directive
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
    virtual const hyper::Token& currentToken() const = 0;
    virtual const hyper::Token& lookaheadToken(std::size_t distance) const = 0;
    virtual void advanceToken() = 0;
    virtual bool consume(hyper::TokenKind kind) = 0;
    virtual bool expect(hyper::TokenKind kind) = 0;
};

} // namespace hyper::parse
