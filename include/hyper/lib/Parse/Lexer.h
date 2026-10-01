#pragma once
#ifndef HYPER_LIB_PARSE_LEXER_H
#define HYPER_LIB_PARSE_LEXER_H
#include "hyper/lib/Parse/Token.h"
#include <cstdint>
#include <functional>
#include <string>
#include <string_view>
#include <vector>
namespace hyper::parse {
enum class DiagnosticSeverity : std::uint8_t { Note, Warning, Error, Fatal };
struct LexerDiagnostic {
 DiagnosticSeverity severity=DiagnosticSeverity::Error; SourceRange range{}; std::string message{};
};
struct LexerOptions {
 bool preserveComments=false;
 bool allowUnicodeIdentifiers=true;
 bool allowNumericSeparators=true;
 bool allowBinaryLiterals=true;
 bool allowOctalLiterals=true;
 bool allowHexLiterals=true;
 bool allowScientificNotation=true;
 bool diagnoseUnknownUnicode=false;
};
class Lexer {
public:
 using DiagnosticHandler=std::function<void(const LexerDiagnostic&)>;
 Lexer(SourceManager& sources,SourceManager::FileID file,LexerOptions options={});
 Lexer(std::string_view source,LexerOptions options={});
 void reset(SourceManager::FileID file);
 void reset(std::string_view source);
 Token lex();
 std::vector<Token> lexAll();
 const Token& currentToken()const noexcept;
 const std::vector<LexerDiagnostic>& diagnostics()const noexcept;
 void setDiagnosticHandler(DiagnosticHandler handler);
 bool hasErrors()const noexcept;
 bool hadFatalError()const noexcept;
 SourceOffset offset()const noexcept;
 SourcePosition position()const noexcept;
 SourceManager::FileID fileID()const noexcept;
private:
 SourceManager* sources_=nullptr; SourceManager::FileID file_=0; SourceText ownedSource_{};
 std::string_view input_{}; LexerOptions options_{}; DiagnosticHandler diagnosticHandler_{};
 std::vector<LexerDiagnostic> diagnostics_{}; Token current_{};
 SourceOffset cursor_=0; SourceOffset tokenStart_=0; SourceLine line_=1; SourceColumn column_=1;
 bool fatal_=false; bool reachedEOF_=false;
 char peek(std::size_t lookahead=0)const noexcept; bool atEnd()const noexcept;
 char consume(); bool consumeIf(char); bool consumeIf(std::string_view);
 void skipWhitespace(); bool skipLineComment(); bool skipBlockComment();
 Token lexIdentifierOrKeyword(); Token lexNumber(); Token lexString(); Token lexCharacter();
 Token lexOperatorOrPunctuation(); Token makeToken(TokenKind,SourceOffset)const;
 Token makeSimple(TokenKind);
 void error(std::string); void errorAt(SourceRange,std::string);
 bool isIdentifierStart(unsigned char)const noexcept; bool isIdentifierContinue(unsigned char)const noexcept;
 bool lexUTF8Identifier(); std::uint32_t decodeUTF8(SourceOffset,std::size_t&)const noexcept;
 bool lexEscape(std::string&); bool lexHexEscape(std::string&,std::size_t);
 bool lexUnicodeEscape(std::string&,std::size_t);
 bool isDecimalDigit(char)const noexcept; bool isHexDigit(char)const noexcept; bool isBinaryDigit(char)const noexcept; bool isOctalDigit(char)const noexcept;
 void consumeDigits(int base); bool consumeNumericSeparator();
 TokenKind lookupKeyword(std::string_view)const noexcept; TokenKind lookupSpecialKeyword(std::string_view)const noexcept;
};
}
#endif
