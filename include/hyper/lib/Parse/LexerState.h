#pragma once
#ifndef HYPER_LIB_PARSE_LEXER_STATE_H
#define HYPER_LIB_PARSE_LEXER_STATE_H

#include "hyper/lib/Parse/SourceLocation.h"

namespace hyper::parse {

class LexerState {
public:
    LexerState() = default;

    bool isValid() const noexcept { return valid_; }
    SourceOffset offset() const noexcept { return cursor_; }
    SourceOffset tokenStart() const noexcept { return tokenStart_; }
    SourceLine line() const noexcept { return line_; }
    SourceColumn column() const noexcept { return column_; }
    bool hadFatalError() const noexcept { return fatal_; }
    bool reachedEOF() const noexcept { return reachedEOF_; }

private:
    SourceOffset cursor_ = 0;
    SourceOffset tokenStart_ = 0;
    SourceLine line_ = 1;
    SourceColumn column_ = 1;
    bool fatal_ = false;
    bool reachedEOF_ = false;
    bool valid_ = false;

    LexerState(SourceOffset cursor, SourceOffset tokenStart,
                SourceLine line, SourceColumn column,
                bool fatal, bool reachedEOF)
        : cursor_(cursor),
          tokenStart_(tokenStart),
          line_(line),
          column_(column),
          fatal_(fatal),
          reachedEOF_(reachedEOF),
          valid_(true) {}

    friend class Lexer;
};

} // namespace hyper::parse

#endif
