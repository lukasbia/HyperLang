#include "hyper/lib/Parse/Lexer.h"
#include <algorithm>
#include <array>
#include <cctype>
#include <string>
#include <unordered_map>
namespace hyper::parse {
namespace {
struct KeywordEntry { std::string_view spelling; TokenKind kind; };
constexpr std::array<KeywordEntry,85> keywordTable={{
{"function", TokenKind::Keyword},
{"var", TokenKind::Keyword},
{"const", TokenKind::Keyword},
{"struct", TokenKind::Keyword},
{"class", TokenKind::Keyword},
{"enum", TokenKind::Keyword},
{"protocol", TokenKind::Keyword},
{"extension", TokenKind::Keyword},
{"type", TokenKind::Keyword},
{"typealias", TokenKind::Keyword},
{"template", TokenKind::Keyword},
{"typename", TokenKind::Keyword},
{"operator", TokenKind::Keyword},
{"namespace", TokenKind::Keyword},
{"module", TokenKind::Keyword},
{"init", TokenKind::Keyword},
{"deinit", TokenKind::Keyword},
{"self", TokenKind::Keyword},
{"super", TokenKind::Keyword},
{"new", TokenKind::Keyword},
{"get", TokenKind::Keyword},
{"set", TokenKind::Keyword},
{"if", TokenKind::Keyword},
{"elseif", TokenKind::Keyword},
{"else", TokenKind::Keyword},
{"endif", TokenKind::Keyword},
{"switch", TokenKind::Keyword},
{"case", TokenKind::Keyword},
{"default", TokenKind::Keyword},
{"while", TokenKind::Keyword},
{"loop", TokenKind::Keyword},
{"do", TokenKind::Keyword},
{"for", TokenKind::Keyword},
{"in", TokenKind::Keyword},
{"break", TokenKind::Keyword},
{"continue", TokenKind::Keyword},
{"return", TokenKind::Keyword},
{"throw", TokenKind::Keyword},
{"try", TokenKind::Keyword},
{"catch", TokenKind::Keyword},
{"finally", TokenKind::Keyword},
{"defer", TokenKind::Keyword},
{"error", TokenKind::Keyword},
{"panic", TokenKind::Keyword},
{"bool", TokenKind::Keyword},
{"int", TokenKind::Keyword},
{"uint", TokenKind::Keyword},
{"int8", TokenKind::Keyword},
{"int16", TokenKind::Keyword},
{"int32", TokenKind::Keyword},
{"int64", TokenKind::Keyword},
{"uint8", TokenKind::Keyword},
{"uint16", TokenKind::Keyword},
{"uint32", TokenKind::Keyword},
{"uint64", TokenKind::Keyword},
{"float", TokenKind::Keyword},
{"double", TokenKind::Keyword},
{"byte", TokenKind::Keyword},
{"char", TokenKind::Keyword},
{"string", TokenKind::Keyword},
{"void", TokenKind::Keyword},
{"nil", TokenKind::Keyword},
{"public", TokenKind::Keyword},
{"private", TokenKind::Keyword},
{"protected", TokenKind::Keyword},
{"internal", TokenKind::Keyword},
{"static", TokenKind::Keyword},
{"extern", TokenKind::Keyword},
{"inline", TokenKind::Keyword},
{"volatile", TokenKind::Keyword},
{"sizeof", TokenKind::Keyword},
{"async", TokenKind::Keyword},
{"await", TokenKind::Keyword},
{"task", TokenKind::Keyword},
{"actor", TokenKind::Keyword},
{"parallel", TokenKind::Keyword},
{"atomic", TokenKind::Keyword},
{"input", TokenKind::Keyword},
{"output", TokenKind::Keyword},
{"message", TokenKind::Keyword},
{"show", TokenKind::Keyword},
{"getData", TokenKind::Keyword},
{"createData", TokenKind::Keyword},
{"from", TokenKind::Keyword},
{"import", TokenKind::Keyword}
}};
constexpr std::array<KeywordEntry,9> specialTable={{
{"@API", TokenKind::AtApi},
{"@APIID", TokenKind::AtApiId},
{"@file", TokenKind::AtFile},
{"@filename", TokenKind::AtFilename},
{"@fileID", TokenKind::AtFileId},
{"@audio", TokenKind::AtAudio},
{"@playAudio", TokenKind::AtPlayAudio},
{"@image", TokenKind::AtImage},
{"@imageID", TokenKind::AtImageId}
}};
}
Lexer::Lexer(SourceManager&s,SourceManager::FileID f,LexerOptions o):sources_(&s),file_(f),options_(o){reset(f);}
Lexer::Lexer(std::string_view s,LexerOptions o):options_(o){reset(s);}
void Lexer::reset(SourceManager::FileID f){file_=f;input_=sources_?sources_->source(f).view():std::string_view{};cursor_=0;tokenStart_=0;line_=1;column_=1;fatal_=false;reachedEOF_=false;diagnostics_.clear();current_={};}
void Lexer::reset(std::string_view s){sources_=nullptr;file_=0;ownedSource_.reset(s);input_=ownedSource_.view();cursor_=0;tokenStart_=0;line_=1;column_=1;fatal_=false;reachedEOF_=false;diagnostics_.clear();current_={};}
char Lexer::peek(std::size_t n)const noexcept{return cursor_+n<input_.size()?input_[cursor_+n]:'\0';}
bool Lexer::atEnd()const noexcept{return cursor_>=input_.size();}
char Lexer::consume(){if(atEnd())return '\0';char c=input_[cursor_++];if(c=='\n'){++line_;column_=1;}else{++column_;}return c;}
bool Lexer::consumeIf(char c){if(peek()==c){consume();return true;}return false;}
bool Lexer::consumeIf(std::string_view s){if(input_.substr(cursor_,s.size())==s){for(char c:s)(void)c,consume();return true;}return false;}
void Lexer::skipWhitespace(){
 while(!atEnd()){
  unsigned char c=static_cast<unsigned char>(peek());
  if(c==' '||c=='\t'||c=='\r'||c=='\n'||c=='\f'||c=='\v'){consume();continue;}
  if(static_cast<unsigned char>(peek())>=0x80){std::size_t n=0;auto cp=decodeUTF8(cursor_,n);if(cp==0xFEFF){while(n--)consume();continue;}}
  break;
 }
}
bool Lexer::skipLineComment(){if(!consumeIf("//"))return false;while(!atEnd()&&peek()!='\n')consume();return true;}
bool Lexer::skipBlockComment(){
 if(!consumeIf("/*"))return false;SourceOffset start=tokenStart_;
 while(!atEnd()){if(consumeIf("*/"))return true;consume();}
 errorAt(makeToken(TokenKind::Unknown,start).range,"unterminated block comment");fatal_=true;return true;
}
Token Lexer::makeToken(TokenKind k,SourceOffset start)const{
 SourcePosition b{start,0,0},e{cursor_,0,0};
 if(sources_&&file_) {b=sources_->position(file_,start);e=sources_->position(file_,cursor_);}
 else {b={start,1,static_cast<SourceColumn>(start+1)};e={cursor_,line_,column_};}
 return {k,{b,e},std::string(input_.substr(start,cursor_-start))};
}
Token Lexer::makeSimple(TokenKind k){return makeToken(k,tokenStart_);}
Token Lexer::lex(){
 if(reachedEOF_)return current_;
 for(;;){
  tokenStart_=cursor_;
  skipWhitespace();
  tokenStart_=cursor_;
  if(atEnd()){reachedEOF_=true;current_=makeToken(TokenKind::Eof,cursor_);return current_;}
  if(skipLineComment()||skipBlockComment()){if(options_.preserveComments){}continue;}
  break;
 }
 tokenStart_=cursor_;
 unsigned char c=static_cast<unsigned char>(peek());
 if(isIdentifierStart(c)) current_=lexIdentifierOrKeyword();
 else if(std::isdigit(c)|| (c=='.'&&std::isdigit(static_cast<unsigned char>(peek(1))))) current_=lexNumber();
 else if(c=='"') current_=lexString();
 else if(c=='\''){current_=lexCharacter();}
 else current_=lexOperatorOrPunctuation();
 return current_;
}
std::vector<Token> Lexer::lexAll(){std::vector<Token> out;while(true){Token t=lex();out.push_back(t);if(t.isEOF())break;}return out;}
const Token& Lexer::currentToken()const noexcept{return current_;}
const std::vector<LexerDiagnostic>& Lexer::diagnostics()const noexcept{return diagnostics_;}
void Lexer::setDiagnosticHandler(DiagnosticHandler h){diagnosticHandler_=std::move(h);}
bool Lexer::hasErrors()const noexcept{return std::any_of(diagnostics_.begin(),diagnostics_.end(),[](const auto&d){return d.severity==DiagnosticSeverity::Error||d.severity==DiagnosticSeverity::Fatal;});}
bool Lexer::hadFatalError()const noexcept{return fatal_;}
SourceOffset Lexer::offset()const noexcept{return cursor_;}
SourcePosition Lexer::position()const noexcept{return sources_&&file_?sources_->position(file_,cursor_):SourcePosition{cursor_,line_,column_};}
SourceManager::FileID Lexer::fileID()const noexcept{return file_;}
bool Lexer::isIdentifierStart(unsigned char c)const noexcept{return std::isalpha(c)||c=='_'||c>=0x80;}
bool Lexer::isIdentifierContinue(unsigned char c)const noexcept{return std::isalnum(c)||c=='_'||c>=0x80;}
bool Lexer::isDecimalDigit(char c)const noexcept{return c>='0'&&c<='9';}
bool Lexer::isHexDigit(char c)const noexcept{return std::isxdigit(static_cast<unsigned char>(c));}
bool Lexer::isBinaryDigit(char c)const noexcept{return c=='0'||c=='1';}
bool Lexer::isOctalDigit(char c)const noexcept{return c>='0'&&c<='7';}
void Lexer::consumeDigits(int base){for(;;){char c=peek();bool ok=base==10?isDecimalDigit(c):base==16?isHexDigit(c):base==8?isOctalDigit(c):isBinaryDigit(c);if(ok){consume();continue;}if(c=='_'&&options_.allowNumericSeparators){consume();continue;}break;}}
bool Lexer::consumeNumericSeparator(){return options_.allowNumericSeparators&&consumeIf('_');}
TokenKind Lexer::lookupKeyword(std::string_view s)const noexcept{for(auto&e:keywordTable)if(e.spelling==s)return e.kind;return TokenKind::Identifier;}
TokenKind Lexer::lookupSpecialKeyword(std::string_view s)const noexcept{for(auto&e:specialTable)if(e.spelling==s)return e.kind;return TokenKind::Identifier;}
}
