#include "hyper/lib/Parse/Lexer.cpp"
#include <iostream>
#include <string>
using namespace hyper::parse;
static int failures=0;
static void expect(bool value,const char*name){if(!value){std::cerr<<"FAIL: "<<name<<"\n";++failures;}}
static void expectKind(const std::vector<Token>&ts,std::size_t i,TokenKind k,const char*name){expect(i<ts.size()&&ts[i].kind==k,name);}
int main(){
 Lexer lexer(R"(function hello(name) { var message: string = "Hello, 世界"; return message })");
 auto tokens=lexer.lexAll();
 expectKind(tokens,0,TokenKind::Keyword,"function keyword");
 expectKind(tokens,1,TokenKind::Identifier,"function name");
 expectKind(tokens,2,TokenKind::LParen,"left parenthesis");
 expectKind(tokens,3,TokenKind::Identifier,"parameter");
 expectKind(tokens,4,TokenKind::RParen,"right parenthesis");
 expectKind(tokens,5,TokenKind::LBrace,"left brace");
 expectKind(tokens,6,TokenKind::Keyword,"var keyword");
 expectKind(tokens,7,TokenKind::Identifier,"variable");
 expectKind(tokens,8,TokenKind::Colon,"colon");
 expectKind(tokens,9,TokenKind::Keyword,"string type");
 expectKind(tokens,10,TokenKind::Equal,"assignment");
 expectKind(tokens,11,TokenKind::StringLiteral,"unicode string");
 expectKind(tokens.back(),TokenKind::Eof,"eof");
 Lexer numbers(R"(0 42 0xff 0b1010 0o77 1.25 2e10)");
 auto ns=numbers.lexAll(); for(std::size_t i=0;i<7;++i)expect(ns[i].kind==TokenKind::IntegerLiteral||ns[i].kind==TokenKind::FloatingLiteral,"numeric literal");
 Lexer ops(R"(== != <= >= += -= *= /= %= && || -> => .. ... ?? ?.)");
 auto os=ops.lexAll();
 expectKind(os,0,TokenKind::EqualEqual,"=="); expectKind(os,1,TokenKind::BangEqual,"!=");
 expectKind(os,2,TokenKind::LessEqual,"<="); expectKind(os,3,TokenKind::GreaterEqual,">=");
 expectKind(os,4,TokenKind::PlusEqual,"+="); expectKind(os,5,TokenKind::MinusEqual,"-=");
 expectKind(os,6,TokenKind::StarEqual,"*="); expectKind(os,7,TokenKind::SlashEqual,"/=");
 expectKind(os,8,TokenKind::PercentEqual,"%="); expectKind(os,9,TokenKind::AmpAmp,"&&");
 expectKind(os,10,TokenKind::PipePipe,"||"); expectKind(os,11,TokenKind::Arrow,"->");
 expectKind(os,12,TokenKind::FatArrow,"=>"); expectKind(os,13,TokenKind::Range,"..");
 expectKind(os,14,TokenKind::Ellipsis,"..."); expectKind(os,15,TokenKind::NullCoalescing,"??");
 expectKind(os,16,TokenKind::QuestionDot,"?.");
 Lexer api(R"(struct api { @APIID(1234567891011) @API = ai })");
 auto as=api.lexAll(); expectKind(as,3,TokenKind::AtApiId,"@APIID"); expectKind(as,7,TokenKind::AtApi,"@API");
 Lexer comments(R"(// ignored
/* ignored */ function x() {})");
 auto cs=comments.lexAll(); expectKind(cs,0,TokenKind::Keyword,"comments skipped");
 Lexer escapes(R"("a
b	A😀")"); auto es=escapes.lexAll();expectKind(es,0,TokenKind::StringLiteral,"escapes");
 return failures?1:0;
}
