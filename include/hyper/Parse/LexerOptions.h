#pragma once

namespace hyper {

struct LexerOptions {
    bool keepComments = false;
    bool allowHashbang = true;
    bool allowRegexLiterals = false;
    bool treatEditorPlaceholdersAsTokens = true;
    bool preserveTrivia = false;
    bool diagnoseUTF8Errors = true;
    bool diagnoseNumericSeparators = true;
    bool diagnoseStringEscapes = true;
    bool diagnoseOperatorSpacing = true;
};

} // namespace hyper
