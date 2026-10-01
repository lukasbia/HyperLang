#pragma once
#ifndef HYPER_LIB_PARSE_LEXER_STATE_H
#define HYPER_LIB_PARSE_LEXER_STATE_H
#include "hyper/lib/Parse/SourceLocation.h"
namespace hyper::parse {
struct LexerState {
    SourceOffset offset = 0;
    SourceLine line = 1;
    SourceColumn column = 1;
    bool valid = false;
    LexerState() = default;
    LexerState(SourceOffset o, SourceLine l, SourceColumn c)
        : offset(o), line(l), column(c), valid(true) {}
};
}
#endif
