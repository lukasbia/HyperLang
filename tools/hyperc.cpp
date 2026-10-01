#include "hyper/Compiler/Compiler.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
int main(int argc,char** argv){
 if(argc<2){std::cerr<<"usage: hyperc <file>\n";return 2;}
 std::ifstream in(argv[1],std::ios::binary);
 if(!in){std::cerr<<"hyperc: cannot open "<<argv[1]<<"\n";return 1;}
 std::string source((std::istreambuf_iterator<char>(in)),{});
 hyper::compiler::Compiler compiler;
 auto result=compiler.compile(source,argv[1]);
 for(const auto& d:result.diagnostics)std::cerr<<argv[1]<<": "<<d.message<<"\n";
 if(!result.success)return 1;
 std::cout<<"HyperLang compiled "<<argv[1]<<" to "<<result.machineCode.bytes.size()<<" backend bytes\n";
 return 0;
}
