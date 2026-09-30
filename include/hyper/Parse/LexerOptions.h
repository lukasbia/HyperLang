#pragma once

#include <cstddef>

namespace hyper {

struct LexerOptions {
    bool keepComments = false;
    bool allowHashbang = true;
    bool allowRegexLiterals = false;
    bool treatEditorPlaceholdersAsTokens = true;
    bool allowUnicodeIdentifiers = true;
    bool allowEscapedIdentifiers = true;
    bool allowDollarIdentifiers = true;
    bool allowMultilineStrings = true;
    bool allowRawStrings = true;
    bool allowNumericSeparators = true;
    bool allowHexFloatingPoint = true;
    bool recoverMalformedTokens = true;
    bool preserveWhitespace = false;
    bool preserveNewlines = false;
    std::size_t maximumLookahead = 256;
};

} // namespace hyper
