#include "hyperlang/Lexer/Lexer.h"
#include "hyperlang/Lexer/Token.h"
#include "hyperlang/AST/AST.h"
#include "hyperlang/HIL/HIL.h"
#include "hyperlang/MCM/MCM.h"
#include "hyperlang/LSC/LSC.h"
#include <cctype>
#include <unordered_map>
namespace hyperlang::lexer {
static const std::unordered_map<std::string,TokenKind> keywords={
 {"func",TokenKind::Func},{"else",TokenKind::Else},{"if",TokenKind::If},{"then",TokenKind::Then},{"endif",TokenKind::EndIf},{"while",TokenKind::While},
 {"true",TokenKind::True},{"false",TokenKind::False},{"do",TokenKind::Do},{"loop",TokenKind::Loop},{"endLoop",TokenKind::EndLoop},{"let",TokenKind::Let},{"var",TokenKind::Var},{"string",TokenKind::String},
 {"panic",TokenKind::Panic},{"input",TokenKind::Input},{"output",TokenKind::Output},{"init",TokenKind::Init},{"deinit",TokenKind::Deinit},{"int",TokenKind::Int},{"num",TokenKind::Num},{"enum",TokenKind::Enum},{"nil",TokenKind::Nil},
 {"class",TokenKind::Class},{"struct",TokenKind::Struct},{"protocol",TokenKind::Protocol},{"extension",TokenKind::Extension},{"typealias",TokenKind::Typealias},{"guard",TokenKind::Guard},{"switch",TokenKind::Switch},{"case",TokenKind::Case},{"default",TokenKind::Default},
 {"for",TokenKind::For},{"in",TokenKind::In},{"break",TokenKind::Break},{"continue",TokenKind::Continue},{"return",TokenKind::Return},{"defer",TokenKind::Defer},{"throw",TokenKind::Throw},{"throws",TokenKind::Throws},{"catch",TokenKind::Catch},
 {"async",TokenKind::Async},{"await",TokenKind::Await},{"some",TokenKind::Some},{"any",TokenKind::Any},{"self",TokenKind::Self},{"where",TokenKind::Where},{"get",TokenKind::Get},{"set",TokenKind::Set},{"mutating",TokenKind::Mutating},{"static",TokenKind::Static},{"final",TokenKind::Final},
 {"private",TokenKind::Private},{"public",TokenKind::Public},{"internal",TokenKind::Internal},{"operator",TokenKind::Operator},{"subscript",TokenKind::Subscript},{"associatedtype",TokenKind::AssociatedType},{"required",TokenKind::Required},{"convenience",TokenKind::Convenience},{"override",TokenKind::Override},
 {"weak",TokenKind::Weak},{"unowned",TokenKind::Unowned},{"borrow",TokenKind::Borrow},{"consume",TokenKind::Consume},{"yield",TokenKind::Yield},{"macro",TokenKind::Macro},{"attribute",TokenKind::Attribute},
 {"module",TokenKind::Module},{"package",TokenKind::Package},{"namespace",TokenKind::Namespace},{"source",TokenKind::Source},{"file",TokenKind::File},{"function",TokenKind::Function},{"property",TokenKind::Property},{"event",TokenKind::Event},{"signal",TokenKind::Signal},
 {"asynclet",TokenKind::AsyncLet},{"actor",TokenKind::Actor},{"task",TokenKind::Task},{"detach",TokenKind::Detach},{"isolated",TokenKind::Isolated},{"nonisolated",TokenKind::Nonisolated},{"sendable",TokenKind::Sendable},{"move",TokenKind::Move},{"copy",TokenKind::Copy},
 {"weakref",TokenKind::WeakRef},{"strongref",TokenKind::StrongRef},{"own",TokenKind::Own},{"shared",TokenKind::Shared},{"observe",TokenKind::Observe},{"synchronize",TokenKind::Synchronize},{"compile",TokenKind::Compile},{"extern",TokenKind::Extern}
};
Lexer::Lexer(std::string_view s):source_(s){}
char Lexer::peek(std::size_t n)const{return index_+n<source_.size()?source_[index_+n]:'\0';}
char Lexer::advance(){char c=peek();if(c){++index_;if(c=='\n'){++location_.line;location_.column=1;}else ++location_.column;}return c;}
void Lexer::skipWhitespace(){for(;;){while(std::isspace(static_cast<unsigned char>(peek())))advance();if(peek()=='/'&&peek(1)=='/'){while(peek()&&peek()!='\n')advance();continue;}break;}}
Token Lexer::identifier(){auto start=location_;std::size_t b=index_;while(std::isalnum(static_cast<unsigned char>(peek()))||peek()=='_')advance();std::string s(source_.substr(b,index_-b));auto it=keywords.find(s);return {it==keywords.end()?TokenKind::Identifier:it->second,s,start};}
Token Lexer::number(){auto start=location_;std::size_t b=index_;bool dot=false;while(std::isdigit(static_cast<unsigned char>(peek()))||(!dot&&peek()=='.')){if(peek()=='.')dot=true;advance();}return {dot?TokenKind::NumberLiteral:TokenKind::IntegerLiteral,std::string(source_.substr(b,index_-b)),start};}
Token Lexer::string(){auto start=location_;advance();std::string s;while(peek()&&peek()!='"'){if(peek()=='\\'&&peek(1)){advance();s+=advance();}else s+=advance();}if(peek()=='"')advance();return {TokenKind::StringLiteral,s,start};}
Token Lexer::directive(){auto start=location_;advance();std::size_t b=index_;while(std::isalpha(static_cast<unsigned char>(peek())))advance();std::string s(source_.substr(b,index_-b));if(s=="include")return {TokenKind::AtInclude,"@include",start};if(s=="import")return {TokenKind::AtImport,"@import",start};return {TokenKind::AtDirective,"@"+s,start};}
Token Lexer::symbol(){auto start=location_;char c=advance();auto make=[&](TokenKind k){return Token{k,std::string(1,c),start};};switch(c){case '{':return make(TokenKind::LBrace);case '}':return make(TokenKind::RBrace);case '(':return make(TokenKind::LParen);case ')':return make(TokenKind::RParen);case '[':return make(TokenKind::LBracket);case ']':return make(TokenKind::RBracket);case '.':return make(TokenKind::Dot);case ',':return make(TokenKind::Comma);case ':':return make(TokenKind::Colon);case ';':return make(TokenKind::Semicolon);case '+':return make(TokenKind::Plus);case '-':if(peek()=='>'){advance();return {TokenKind::Arrow,"->",start};}return make(TokenKind::Minus);case '*':return make(TokenKind::Star);case '/':return make(TokenKind::Slash);case '%':return make(TokenKind::Percent);case '!':if(peek()=='='){advance();return {TokenKind::NotEqual,"!=",start};}return make(TokenKind::Bang);case '=':if(peek()=='='){advance();return {TokenKind::EqualEqual,"==",start};}return make(TokenKind::Equal);case '<':if(peek()=='='){advance();return {TokenKind::LessEqual,"<=",start};}return make(TokenKind::Less);case '>':if(peek()=='='){advance();return {TokenKind::GreaterEqual,">=",start};}return make(TokenKind::Greater);case '&':if(peek()=='&'){advance();return {TokenKind::AndAnd,"&&",start};}break;case '|':if(peek()=='|'){advance();return {TokenKind::OrOr,"||",start};}break;}return make(TokenKind::Unknown);}
std::vector<Token> Lexer::tokenize(){std::vector<Token> out;while(true){skipWhitespace();char c=peek();if(!c){out.push_back({TokenKind::EndOfFile,"",location_});break;}if(c=='@')out.push_back(directive());else if(std::isalpha(static_cast<unsigned char>(c))||c=='_')out.push_back(identifier());else if(std::isdigit(static_cast<unsigned char>(c)))out.push_back(number());else if(c=='"')out.push_back(string());else out.push_back(symbol());}return out;}
}
