#include "hyper/lib/Parse/Lexer.h"
#include <iostream>
#include <string>
int main(int argc,char**argv){
 if(argc<2){std::cerr<<"usage: hyperlang-lex <source>\n";return 2;}
 hyper::parse::SourceManager sm; auto id=sm.addSource("<command-line>",argv[1]);
 hyper::parse::Lexer lexer(sm,id);
 lexer.setDiagnosticHandler([](const auto&d){std::cerr<<"error: "<<d.message<<" at "<<d.range.begin.line<<":"<<d.range.begin.column<<"\n";});
 for(const auto&t:lexer.lexAll()) std::cout<<hyper::parse::tokenKindName(t.kind)<<" ["<<t.start()<<","<<t.end()<<") "<<t.text<<"\n";
 return lexer.hasErrors()?1:0;
}
