#include "hyper/lib/Parse/Lexer.h"
#include "hyper/lib/Parse/LexerDiagnostics.h"
#include "hyper/lib/Parse/LexerOptions.h"
#include "hyper/lib/Parse/LexerState.h"
#include "hyper/lib/Parse/LexerUnicode.h"
#include <algorithm>
#include <array>
#include <cctype>
#include <limits>
#include <unordered_map>
#include <utility>
namespace hyper::parse {
namespace {
struct KeywordEntry { std::string_view spelling; TokenKind kind; };
constexpr std::array<KeywordEntry,85> keywordTable={{
{"func", TokenKind::Keyword},
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
inline Lexer::Lexer(SourceManager&s,SourceManager::FileID f,LexerOptions o):sources_(&s),file_(f),options_(o){reset(f);}
inline Lexer::Lexer(std::string_view s,LexerOptions o):options_(o){reset(s);}
void inline Lexer::reset(SourceManager::FileID f){file_=f;input_=sources_?sources_->source(f).view():std::string_view{};cursor_=0;tokenStart_=0;line_=1;column_=1;fatal_=false;reachedEOF_=false;diagnostics_.clear();current_={};}
void inline Lexer::reset(std::string_view s){sources_=nullptr;file_=0;ownedSource_.reset(s);input_=ownedSource_.view();cursor_=0;tokenStart_=0;line_=1;column_=1;fatal_=false;reachedEOF_=false;diagnostics_.clear();current_={};}
char inline Lexer::peek(std::size_t n)const noexcept{return cursor_+n<input_.size()?input_[cursor_+n]:'\0';}
bool inline Lexer::atEnd()const noexcept{return cursor_>=input_.size();}
char inline Lexer::consume(){if(atEnd())return '\0';char c=input_[cursor_++];if(c=='\n'){++line_;column_=1;}else{++column_;}return c;}
bool inline Lexer::consumeIf(char c){if(peek()==c){consume();return true;}return false;}
bool inline Lexer::consumeIf(std::string_view s){if(input_.substr(cursor_,s.size())==s){for(char c:s)(void)c,consume();return true;}return false;}
void inline Lexer::skipWhitespace(){
 while(!atEnd()){
  unsigned char c=static_cast<unsigned char>(peek());
  if(c==' '||c=='\t'||c=='\r'||c=='\n'||c=='\f'||c=='\v'){consume();continue;}
  if(static_cast<unsigned char>(peek())>=0x80){std::size_t n=0;auto cp=decodeUTF8(cursor_,n);if(cp==0xFEFF){while(n--)consume();continue;}}
  break;
 }
}
bool inline Lexer::skipLineComment(){if(!consumeIf("//"))return false;while(!atEnd()&&peek()!='\n')consume();return true;}
bool inline Lexer::skipBlockComment(){
 if(!consumeIf("/*"))return false;SourceOffset start=tokenStart_;
 while(!atEnd()){if(consumeIf("*/"))return true;consume();}
 errorAt(makeToken(TokenKind::Unknown,start).range,"unterminated block comment");fatal_=true;return true;
}
Token inline Lexer::makeToken(TokenKind k,SourceOffset start)const{
 SourcePosition b{start,0,0},e{cursor_,0,0};
 if(sources_&&file_) {b=sources_->position(file_,start);e=sources_->position(file_,cursor_);}
 else {b={start,1,static_cast<SourceColumn>(start+1)};e={cursor_,line_,column_};}
 return {k,{b,e},std::string(input_.substr(start,cursor_-start))};
}
Token inline Lexer::makeSimple(TokenKind k){return makeToken(k,tokenStart_);}
Token inline Lexer::lex(){
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
std::vector<Token> inline Lexer::lexAll(){std::vector<Token> out;while(true){Token t=lex();out.push_back(t);if(t.isEOF())break;}return out;}
const Token& inline Lexer::currentToken()const noexcept{return current_;}
const std::vector<LexerDiagnostic>& inline Lexer::diagnostics()const noexcept{return diagnostics_;}
void inline Lexer::setDiagnosticHandler(DiagnosticHandler h){diagnosticHandler_=std::move(h);}
bool inline Lexer::hasErrors()const noexcept{return std::any_of(diagnostics_.begin(),diagnostics_.end(),[](const auto&d){return d.severity==DiagnosticSeverity::Error||d.severity==DiagnosticSeverity::Fatal;});}
bool inline Lexer::hadFatalError()const noexcept{return fatal_;}
SourceOffset inline Lexer::offset()const noexcept{return cursor_;}
SourcePosition inline Lexer::position()const noexcept{return sources_&&file_?sources_->position(file_,cursor_):SourcePosition{cursor_,line_,column_};}
SourceManager::FileID inline Lexer::fileID()const noexcept{return file_;}
bool inline Lexer::isIdentifierStart(unsigned char c)const noexcept{return std::isalpha(c)||c=='_'||c>=0x80;}
bool inline Lexer::isIdentifierContinue(unsigned char c)const noexcept{return std::isalnum(c)||c=='_'||c>=0x80;}
bool inline Lexer::isDecimalDigit(char c)const noexcept{return c>='0'&&c<='9';}
bool inline Lexer::isHexDigit(char c)const noexcept{return std::isxdigit(static_cast<unsigned char>(c));}
bool inline Lexer::isBinaryDigit(char c)const noexcept{return c=='0'||c=='1';}
bool inline Lexer::isOctalDigit(char c)const noexcept{return c>='0'&&c<='7';}
void inline Lexer::consumeDigits(int base){for(;;){char c=peek();bool ok=base==10?isDecimalDigit(c):base==16?isHexDigit(c):base==8?isOctalDigit(c):isBinaryDigit(c);if(ok){consume();continue;}if(c=='_'&&options_.allowNumericSeparators){consume();continue;}break;}}
bool inline Lexer::consumeNumericSeparator(){return options_.allowNumericSeparators&&consumeIf('_');}
TokenKind inline Lexer::lookupKeyword(std::string_view s)const noexcept{for(auto&e:keywordTable)if(e.spelling==s)return e.kind;return TokenKind::Identifier;}
TokenKind inline Lexer::lookupSpecialKeyword(std::string_view s)const noexcept{for(auto&e:specialTable)if(e.spelling==s)return e.kind;return TokenKind::Identifier;}
}



namespace hyper::parse {
namespace {
bool isNameByte(unsigned char c){return std::isalnum(c)||c=='_'||c>=0x80;}
int hexValue(char c){if(c>='0'&&c<='9')return c-'0';if(c>='a'&&c<='f')return c-'a'+10;if(c>='A'&&c<='F')return c-'A'+10;return -1;}
}
Token inline Lexer::lexIdentifierOrKeyword(){
 const auto start=tokenStart_;
 while(!atEnd()){
  unsigned char c=static_cast<unsigned char>(peek());
  if(c<0x80){if(!isNameByte(c))break;consume();continue;}
  std::size_t width=0;auto cp=decodeUTF8(cursor_,width);if(cp==0||width==0)break;
  cursor_+=width;column_+=static_cast<SourceColumn>(width);
 }
 std::string_view spelling=input_.substr(start,cursor_-start);
 TokenKind kind=lookupKeyword(spelling);
 if(kind==TokenKind::Identifier && options_.allowUnicodeIdentifiers && spelling.find_first_of("@")!=std::string_view::npos)
  kind=lookupSpecialKeyword(spelling);
 return makeToken(kind,start);
}
Token inline Lexer::lexNumber(){
 const auto start=tokenStart_;
 bool floating=false;
 if(peek()=='.'){floating=true;consume();consumeDigits(10);}
 else{
  if(peek()=='0'){
   if(peek(1)=='x'||peek(1)=='X'){consume();consume();consumeDigits(16);return makeToken(TokenKind::IntegerLiteral,start);}
   if(peek(1)=='b'||peek(1)=='B'){consume();consume();consumeDigits(2);return makeToken(TokenKind::IntegerLiteral,start);}
   if(peek(1)=='o'||peek(1)=='O'){consume();consume();consumeDigits(8);return makeToken(TokenKind::IntegerLiteral,start);}
  }
  consumeDigits(10);
  if(peek()=='.'&&peek(1)!='.'){floating=true;consume();consumeDigits(10);}
 }
 if((peek()=='e'||peek()=='E')&&options_.allowScientificNotation){
  floating=true;consume();if(peek()=='+'||peek()=='-')consume();if(!isDecimalDigit(peek()))error("expected exponent digits");consumeDigits(10);
 }
 if(std::isalpha(static_cast<unsigned char>(peek()))||peek()=='_')error("invalid character in numeric literal");
 return makeToken(floating?TokenKind::FloatingLiteral:TokenKind::IntegerLiteral,start);
}
bool inline Lexer::lexHexEscape(std::string&out,std::size_t count){
 std::uint32_t value=0;
 for(std::size_t i=0;i<count;++i){int v=hexValue(peek());if(v<0){error("invalid hexadecimal escape");return false;}value=(value<<4)|static_cast<std::uint32_t>(v);consume();}
 out.push_back(static_cast<char>(value&0xff));return true;
}
bool inline Lexer::lexUnicodeEscape(std::string&out,std::size_t count){
 if(!consumeIf('{')){error("expected '{' in unicode escape");return false;}
 std::uint32_t value=0;std::size_t digits=0;
 while(!atEnd()&&peek()!='}'){int v=hexValue(peek());if(v<0||digits>=count){error("invalid unicode escape");return false;}value=(value<<4)|static_cast<std::uint32_t>(v);++digits;consume();}
 if(!consumeIf('}')||digits==0||value>0x10ffff){error("invalid unicode scalar escape");return false;}
 if(value<=0x7f)out.push_back(static_cast<char>(value));
 else if(value<=0x7ff){out.push_back(static_cast<char>(0xc0|(value>>6)));out.push_back(static_cast<char>(0x80|(value&0x3f)));}
 else if(value<=0xffff){out.push_back(static_cast<char>(0xe0|(value>>12)));out.push_back(static_cast<char>(0x80|((value>>6)&0x3f)));out.push_back(static_cast<char>(0x80|(value&0x3f)));}
 else{out.push_back(static_cast<char>(0xf0|(value>>18)));out.push_back(static_cast<char>(0x80|((value>>12)&0x3f)));out.push_back(static_cast<char>(0x80|((value>>6)&0x3f)));out.push_back(static_cast<char>(0x80|(value&0x3f)));}
 return true;
}
bool inline Lexer::lexEscape(std::string&out){
 if(atEnd())return false;char c=consume();
 switch(c){
  case 'n':out+='\n';return true; case 'r':out+='\r';return true; case 't':out+='\t';return true;
  case 'b':out+='\b';return true; case 'f':out+='\f';return true; case 'v':out+='\v';return true;
  case '0':out+='\0';return true; case '\\':out+='\\';return true; case '"':out+='"';return true; case '\'':out+='\'';return true;
  case 'x':return lexHexEscape(out,2); case 'u':return lexUnicodeEscape(out,6);
  case '\n':return true;
  default:error("unknown escape sequence");out.push_back(c);return false;
 }
}
Token inline Lexer::lexString(){
 const auto start=tokenStart_;consume();std::string value;
 while(!atEnd()){
  if(peek()=='"'){consume();return makeToken(TokenKind::StringLiteral,start);}
  if(peek()=='\\'){consume();lexEscape(value);continue;}
  if(peek()=='\n'||peek()=='\r'){error("unterminated string literal");break;}
  value.push_back(consume());
 }
 error("unterminated string literal");return makeToken(TokenKind::Unknown,start);
}
Token inline Lexer::lexCharacter(){
 const auto start=tokenStart_;consume();std::string value;
 if(atEnd()||peek()=='\n'){error("unterminated character literal");return makeToken(TokenKind::Unknown,start);}
 if(peek()=='\\'){consume();lexEscape(value);}else value.push_back(consume());
 if(!consumeIf('\'')){error("character literal must contain one character");while(!atEnd()&&peek()!='\n'&&peek()!='\'')consume();}
 return makeToken(TokenKind::CharacterLiteral,start);
}
}



namespace hyper::parse {
Token inline Lexer::lexOperatorOrPunctuation(){
 const auto start=tokenStart_;
 const auto two=[&](std::string_view s,TokenKind k){if(input_.substr(cursor_,s.size())==s){for(char c:s)consume();return makeToken(k,start);}return Token{};};
 switch(peek()){
  case '(':consume();return makeSimple(TokenKind::LParen); case ')':consume();return makeSimple(TokenKind::RParen);
  case '{':consume();return makeSimple(TokenKind::LBrace); case '}':consume();return makeSimple(TokenKind::RBrace);
  case '[':consume();return makeSimple(TokenKind::LBracket); case ']':consume();return makeSimple(TokenKind::RBracket);
  case ',':consume();return makeSimple(TokenKind::Comma); case ':': if(peek(1)==':'){consume();consume();return makeSimple(TokenKind::DoubleColon);} consume();return makeSimple(TokenKind::Colon);
  case ';':consume();return makeSimple(TokenKind::Semicolon);
  case '?': if(peek(1)=='.'){consume();consume();return makeSimple(TokenKind::QuestionDot);} if(peek(1)=='?'){consume();consume();return makeSimple(TokenKind::NullCoalescing);} consume();return makeSimple(TokenKind::Question);
  case '.': if(peek(1)=='.'&&peek(2)=='.'){consume();consume();consume();return makeSimple(TokenKind::Ellipsis);} if(peek(1)=='.'&&peek(2)=='='){consume();consume();consume();return makeSimple(TokenKind::RangeInclusive);} if(peek(1)=='.'){consume();consume();return makeSimple(TokenKind::Range);} consume();return makeSimple(TokenKind::Dot);
  case '+': if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::PlusEqual);} if(peek(1)=='+'){consume();consume();return makeSimple(TokenKind::Increment);} consume();return makeSimple(TokenKind::Plus);
  case '-': if(peek(1)=='>'){consume();consume();return makeSimple(TokenKind::Arrow);} if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::MinusEqual);} if(peek(1)=='-'){consume();consume();return makeSimple(TokenKind::Decrement);} consume();return makeSimple(TokenKind::Minus);
  case '*': if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::StarEqual);} consume();return makeSimple(TokenKind::Star);
  case '/': if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::SlashEqual);} consume();return makeSimple(TokenKind::Slash);
  case '%': if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::PercentEqual);} consume();return makeSimple(TokenKind::Percent);
  case '&': if(peek(1)=='&'){consume();consume();return makeSimple(TokenKind::AmpAmp);} if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::AmpEqual);} consume();return makeSimple(TokenKind::Ampersand);
  case '|': if(peek(1)=='|'){consume();consume();return makeSimple(TokenKind::PipePipe);} if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::PipeEqual);} if(peek(1)=='>'){consume();consume();return makeSimple(TokenKind::FatArrow);} consume();return makeSimple(TokenKind::Pipe);
  case '^': if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::CaretEqual);} consume();return makeSimple(TokenKind::Caret);
  case '~':consume();return makeSimple(TokenKind::Tilde);
  case '!': if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::BangEqual);} if(peek(1)=='.'){consume();consume();return makeSimple(TokenKind::BangDot);} consume();return makeSimple(TokenKind::Bang);
  case '=': if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::EqualEqual);} if(peek(1)=='>'){consume();consume();return makeSimple(TokenKind::FatArrow);} consume();return makeSimple(TokenKind::Equal);
  case '<': if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::LessEqual);} if(peek(1)=='<'){consume();consume();if(peek()=='='){consume();return makeSimple(TokenKind::ShiftLeftEqual);}return makeSimple(TokenKind::ShiftLeft);} consume();return makeSimple(TokenKind::Less);
  case '>': if(peek(1)=='='){consume();consume();return makeSimple(TokenKind::GreaterEqual);} if(peek(1)=='>'){consume();consume();if(peek()=='='){consume();return makeSimple(TokenKind::ShiftRightEqual);}return makeSimple(TokenKind::ShiftRight);} consume();return makeSimple(TokenKind::Greater);
  case '@': {
    while(peek()=='@'||std::isalpha(static_cast<unsigned char>(peek()))||std::isdigit(static_cast<unsigned char>(peek()))||peek()=='_')consume();
    auto spelling=input_.substr(start,cursor_-start);auto k=lookupSpecialKeyword(spelling);
    if(k!=TokenKind::Identifier)return makeToken(k,start);
    error("unknown @-keyword");return makeToken(TokenKind::Unknown,start);
  }
  default: consume();error("unexpected character");return makeToken(TokenKind::Unknown,start);
 }
}
}



namespace hyper::parse {
std::uint32_t inline Lexer::decodeUTF8(SourceOffset off,std::size_t&width)const noexcept{
 width=0;if(off>=input_.size())return 0;const auto p=[&](std::size_t i){return static_cast<unsigned char>(off+i<input_.size()?input_[off+i]:0);};
 unsigned char b=p(0);
 if(b<0x80){width=1;return b;}
 if((b&0xe0)==0xc0){if((p(1)&0xc0)!=0x80)return 0;width=2;return ((b&0x1f)<<6)|(p(1)&0x3f);}
 if((b&0xf0)==0xe0){if((p(1)&0xc0)!=0x80||(p(2)&0xc0)!=0x80)return 0;width=3;auto cp=((b&0xf)<<12)|((p(1)&0x3f)<<6)|(p(2)&0x3f);return cp<0x800?0:cp;}
 if((b&0xf8)==0xf0){if((p(1)&0xc0)!=0x80||(p(2)&0xc0)!=0x80||(p(3)&0xc0)!=0x80)return 0;width=4;auto cp=((b&7)<<18)|((p(1)&0x3f)<<12)|((p(2)&0x3f)<<6)|(p(3)&0x3f);return cp<0x10000||cp>0x10ffff?0:cp;}
 return 0;
}
bool inline Lexer::lexUTF8Identifier(){
 std::size_t width=0;auto cp=decodeUTF8(cursor_,width);if(cp==0||width==0)return false;cursor_+=width;column_+=static_cast<SourceColumn>(width);return true;
}
}



namespace hyper::parse {
void inline Lexer::error(std::string message){errorAt(makeToken(TokenKind::Unknown,tokenStart_).range,std::move(message));}
void inline Lexer::errorAt(SourceRange range,std::string message){
 LexerDiagnostic d{DiagnosticSeverity::Error,range,std::move(message)};diagnostics_.push_back(d);if(diagnosticHandler_)diagnosticHandler_(diagnostics_.back());
}
}




LexerState Lexer::saveState() const noexcept {
    return LexerState(cursor_, tokenStart_, line_, column_, fatal_, reachedEOF_);
}

void Lexer::restoreState(const LexerState& state) noexcept {
    if (!state.isValid())
        return;
    cursor_ = state.cursor_;
    tokenStart_ = state.tokenStart_;
    line_ = state.line_;
    column_ = state.column_;
    fatal_ = state.fatal_;
    reachedEOF_ = state.reachedEOF_;
    current_ = {};
}
