#include "hyper/lib/Parse/Lexer.h"
#include <cctype>
#include <charconv>
#include <string>
#include <unordered_map>
namespace hyper::parse {
namespace {
bool isNameByte(unsigned char c){return std::isalnum(c)||c=='_'||c>=0x80;}
int hexValue(char c){if(c>='0'&&c<='9')return c-'0';if(c>='a'&&c<='f')return c-'a'+10;if(c>='A'&&c<='F')return c-'A'+10;return -1;}
}
Token Lexer::lexIdentifierOrKeyword(){
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
Token Lexer::lexNumber(){
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
bool Lexer::lexHexEscape(std::string&out,std::size_t count){
 std::uint32_t value=0;
 for(std::size_t i=0;i<count;++i){int v=hexValue(peek());if(v<0){error("invalid hexadecimal escape");return false;}value=(value<<4)|static_cast<std::uint32_t>(v);consume();}
 out.push_back(static_cast<char>(value&0xff));return true;
}
bool Lexer::lexUnicodeEscape(std::string&out,std::size_t count){
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
bool Lexer::lexEscape(std::string&out){
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
Token Lexer::lexString(){
 const auto start=tokenStart_;consume();std::string value;
 while(!atEnd()){
  if(peek()=='"'){consume();return makeToken(TokenKind::StringLiteral,start);}
  if(peek()=='\\'){consume();lexEscape(value);continue;}
  if(peek()=='\n'||peek()=='\r'){error("unterminated string literal");break;}
  value.push_back(consume());
 }
 error("unterminated string literal");return makeToken(TokenKind::Unknown,start);
}
Token Lexer::lexCharacter(){
 const auto start=tokenStart_;consume();std::string value;
 if(atEnd()||peek()=='\n'){error("unterminated character literal");return makeToken(TokenKind::Unknown,start);}
 if(peek()=='\\'){consume();lexEscape(value);}else value.push_back(consume());
 if(!consumeIf('\'')){error("character literal must contain one character");while(!atEnd()&&peek()!='\n'&&peek()!='\'')consume();}
 return makeToken(TokenKind::CharacterLiteral,start);
}
}
