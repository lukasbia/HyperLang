#pragma once
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>
#include "hyper/Parse/LexerToken.h"
namespace hyper {
enum class LexerDiagnosticSeverity { Note, Warning, Error };
struct LexerDiagnostic {
 LexerDiagnosticSeverity severity=LexerDiagnosticSeverity::Error; SourceLocation location; std::string message;
 std::string replacement; std::string category; std::size_t offset=0; std::size_t length=0;
};
struct LexerOptions {
 bool keepComments=false; bool allowHashbang=true; bool allowRegexLiterals=false;
 bool allowUnicodeIdentifiers=true; bool allowEscapedIdentifiers=true; bool allowDollarIdentifiers=true;
 bool allowMultilineStrings=true; bool allowRawStrings=true; bool allowNumericSeparators=true;
 bool recoverMalformedTokens=true; bool recognizeDocumentationComments=true; bool recognizeCompilerDirectives=true;
 bool recognizeEditorPlaceholders=true; std::size_t maximumLookahead=256; std::size_t maximumTokenLength=1024*1024;
 std::size_t maximumDiagnosticCount=256;
};
class Lexer {
public:
 explicit Lexer(std::string_view source);
 Token next(); Token peek(); Token lookahead(std::size_t distance); std::vector<Token> tokenize();
 void reset(); void setOptions(const LexerOptions& options); const LexerOptions& options() const noexcept;
 bool atEnd() const noexcept; std::size_t currentOffset() const noexcept; SourceLocation currentLocation() const noexcept;
 const std::vector<LexerDiagnostic>& diagnostics() const noexcept; void clearDiagnostics();
private:
 std::string_view source_; std::size_t current_=0,line_=1,column_=1; bool atStartOfLine_=true;
 LexerOptions options_{}; std::vector<LexerDiagnostic> diagnostics_; std::vector<Token> lookaheadBuffer_;
 char peekCharacter(std::size_t distance=0) const noexcept; char advanceCharacter() noexcept;
 bool matchCharacter(char) noexcept; bool startsWith(std::string_view) const noexcept;
 Token lexIdentifierOrKeyword(); Token lexNumber(); Token lexString(); Token lexCharacter(); Token lexRegex();
 Token lexDirective(); Token lexOperator(); Token lexUnknown();
 void consumeTrivia(); bool consumeLineComment(); bool consumeBlockComment(); bool consumeHashbang();
 void diagnose(LexerDiagnosticSeverity,SourceLocation,std::string,std::size_t length=1);
};
} // namespace hyper
