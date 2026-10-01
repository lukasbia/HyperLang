#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
#include "hyper/Parse/LexerToken.h"
namespace hyper {
struct LexerDiagnostic {
 enum class Severity { Note, Warning, Error };
 Severity severity=Severity::Error; SourceLocation location; std::string message; std::string replacement;
};
struct LexerOptions {
 bool keepComments=false; bool allowHashbang=true; bool allowRegexLiterals=false; bool treatEditorPlaceholdersAsTokens=true;
 bool allowUnicodeIdentifiers=true; bool allowEscapedIdentifiers=true; bool allowDollarIdentifiers=true; bool allowMultilineStrings=true;
 bool allowRawStrings=true; bool allowNumericSeparators=true; bool allowHexFloatingPoint=true; bool recoverMalformedTokens=true;
 bool preserveWhitespace=false; bool preserveNewlines=false; bool emitEndOfFileToken=true; bool emitTriviaTokens=false;
 bool recognizeDocumentationComments=true; bool recognizeCompilerDirectives=true; bool recognizeEditorPlaceholders=true;
 bool recognizeAttributeNames=true; std::size_t maximumLookahead=256; std::size_t maximumTokenLength=1024*1024;
 std::size_t maximumDiagnosticCount=256;
};
class Lexer {
public:
 explicit Lexer(std::string_view source);
 Token next(); Token peek(); Token lookahead(std::size_t distance); std::vector<Token> tokenize();
 void reset(); void setKeepComments(bool); void setAllowHashbang(bool); void setAllowRegexLiterals(bool);
 void setTreatEditorPlaceholdersAsTokens(bool); bool keepComments() const noexcept; bool allowHashbang() const noexcept;
 bool allowRegexLiterals() const noexcept; bool atEnd() const noexcept; std::size_t currentOffset() const noexcept;
 SourceLocation currentLocation() const noexcept; const std::vector<LexerDiagnostic>& diagnostics() const noexcept;
 void clearDiagnostics();
private:
 std::string_view source_; std::size_t current_=0,line_=1,column_=1; LexerOptions options_{}; std::vector<LexerDiagnostic> diagnostics_;
 std::vector<Token> lookaheadBuffer_;
 char peekCharacter(std::size_t distance=0) const noexcept; char advance() noexcept; bool match(char) noexcept; bool startsWith(std::string_view) const noexcept;
 SourceLocation locationFromOffset(std::size_t) const; Token makeToken(TokenKind,std::size_t,SourceLocation,bool=false) const;
 void diagnose(LexerDiagnostic::Severity,SourceLocation,std::string,std::string={});
 void diagnoseError(SourceLocation,std::string,std::string={}); void diagnoseWarning(SourceLocation,std::string,std::string={});
 bool isWhitespaceAt(std::size_t) const noexcept; bool isLineBreakAt(std::size_t) const noexcept;
 bool lexLineComment(); bool lexBlockComment(); bool lexHashbang(); bool lexConflictMarker(); bool lexCommentTrivia(); void consumeTrivia();
 Token lexIdentifierOrKeyword(); Token lexEscapedIdentifier(); Token lexDollarIdentifier();
 bool scanDecimalDigits(std::size_t&,bool,std::size_t&,bool&); bool scanBasedDigits(std::size_t&,NumberBase,std::size_t&,bool&);
 Token lexBinaryNumber(); Token lexOctalNumber(); Token lexHexNumber(); Token lexDecimalNumber(); Token lexNumber();
 bool scanUnicodeEscape(std::size_t&,std::uint32_t&); bool scanEscapeSequence(std::size_t&,std::string&,bool&);
 bool scanStringDelimiter(std::size_t&,bool&); bool scanInterpolatedExpression(std::size_t,std::size_t&);
 Token lexString(); Token lexRawString(unsigned); Token lexCharacter(); Token lexDirective(); Token lexHashConstruct();
 bool looksLikeEditorPlaceholder(std::string_view) noexcept; bool looksLikeDirective(std::string_view) noexcept; bool looksLikeConflictMarker(std::string_view) noexcept;
 Token lexRegex(); Token lexOperator(); OperatorBinding classifyOperatorBinding(std::string_view,std::size_t,std::size_t) noexcept; Token lexUnknown();
 void invalidateLookahead();
 static bool isIdentifier(std::string_view); static bool isOperator(std::string_view); static bool isValidEscapedIdentifier(std::string_view);
 static bool isEscapedIdentifierEntirelyWhitespace(std::string_view); static bool isASCIIIdentifierStart(char) noexcept; static bool isASCIIIdentifierContinue(char) noexcept;
 static bool isASCIIDigit(char) noexcept; static bool isASCIIHexDigit(char) noexcept; static bool isASCIIOctalDigit(char) noexcept; static bool isASCIIWhitespace(char) noexcept;
 static bool isPrintableASCII(char) noexcept; static bool isUnicodeIdentifierStart(std::uint32_t) noexcept; static bool isUnicodeIdentifierContinue(std::uint32_t) noexcept;
 static bool isRawIdentifierWhitespace(std::uint32_t) noexcept; static bool isForbiddenIdentifierCodePoint(std::uint32_t) noexcept;
 static bool isOperatorCharacter(char) noexcept; static bool isOperatorStartCharacter(char) noexcept; static bool isOperatorContinuationCharacter(char) noexcept;
 static bool encodeUTF8(std::uint32_t,std::string&); static std::uint32_t validateUTF8Character(std::string_view,std::size_t&);
 static TokenKind keywordKind(std::string_view) noexcept; static TokenKind punctuationKind(std::string_view) noexcept; static TokenKind operatorKind(std::string_view) noexcept;
};
const char* tokenKindName(TokenKind) noexcept;
} // namespace hyper
