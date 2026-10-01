#include "hyper/Compiler/Compiler.h"
#include "hyper/LLCVMBackend/Backend.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
int main(int argc,char** argv){
 if(argc<2){std::cerr<<"usage: hyperc <file> [--target=x86_64] [--hex]\n";return 2;}
 std::string target="x86_64"; bool hex=false;
 for(int i=2;i<argc;++i){std::string a=argv[i];if(a.rfind("--target=",0)==0)target=a.substr(9);else if(a=="--hex")hex=true;}
 std::ifstream in(argv[1],std::ios::binary); if(!in){std::cerr<<"hyperc: cannot open "<<argv[1]<<"\n";return 1;}
 std::string source((std::istreambuf_iterator<char>(in)),{});
 hyper::compiler::Options options; options.target=hyper::llcvm::backend::parseTarget(target);
 hyper::compiler::Compiler compiler(options); auto result=compiler.compile(source,argv[1]);
 for(const auto& d:result.diagnostics)std::cerr<<argv[1]<<": "<<d.message<<"\n";
 if(!result.success)return 1;
 if(hex)std::cout<<hyper::llcvm::backend::emitHex(result.machineCode)<<"\n";
 else std::cout<<"HyperLang -> HIL -> HIL optimization -> LSC IR -> "<<hyper::llcvm::backend::targetName(result.machineCode.target)<<" ("<<result.machineCode.bytes.size()<<" bytes)\n";
 return 0;
}
