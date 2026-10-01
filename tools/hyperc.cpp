#include "hyper/Parse/Parser.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
int main(int argc,char** argv){
 if(argc<2){std::cerr<<"usage: hyperc <file>\n";return 2;}
 std::ifstream in(argv[1],std::ios::binary); if(!in){std::cerr<<"hyperc: cannot open "<<argv[1]<<"\n";return 1;}
 std::string source((std::istreambuf_iterator<char>(in)),{}); hyper::Parser parser(source); auto tree=parser.parse();
 for(const auto& d:parser.diagnostics()) if(d.severity==hyper::ParseDiagnostic::Severity::Error||d.severity==hyper::ParseDiagnostic::Severity::Fatal) std::cerr<<argv[1]<<":"<<d.location.line<<":"<<d.location.column<<": "<<d.message<<"\n";
 return parser.hasErrors()?1:0;
}
