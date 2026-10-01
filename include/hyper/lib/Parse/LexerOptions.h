#pragma once
#ifndef HYPER_LIB_PARSE_LEXER_OPTIONS_H
#define HYPER_LIB_PARSE_LEXER_OPTIONS_H
namespace hyper::parse {
struct LexerOptions {
    bool preserveComments = false;
    bool allowUnicodeIdentifiers = true;
    bool allowNumericSeparators = true;
    bool allowBinaryLiterals = true;
    bool allowOctalLiterals = true;
    bool allowHexLiterals = true;
    bool allowScientificNotation = true;
    bool diagnoseUnknownUnicode = false;
};
}
#endif
