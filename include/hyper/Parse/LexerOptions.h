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
    bool emitEndOfFileToken = true;
    bool emitTriviaTokens = false;
    bool recognizeDocumentationComments = true;
    bool recognizeCompilerDirectives = true;
    bool recognizeEditorPlaceholders = true;
    bool recognizeAttributeNames = true;
    std::size_t maximumLookahead = 256;
    std::size_t maximumTokenLength = 1024 * 1024;
    std::size_t maximumDiagnosticCount = 256;
};

} // namespace hyper
