#include "hyperlang/Lexer/Lexer.h"
#include "hyperlang/Lexer/Token.h"
#include "hyperlang/Parser/Parser.h"
#include "hyperlang/AST/AST.h"
#include "hyperlang/Sema/Sema.h"
#include "hyperlang/HIL/HIL.h"
#include "hyperlang/LSC/LSC.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
int main(int argc,char** argv){if(argc!=2){std::cerr<<"usage: hlc <file.hl>\n";return 1;}std::ifstream file(argv[1]);if(!file){std::cerr<<"cannot open source file\n";return 1;}std::stringstream buffer;buffer<<file.rdbuf();hyperlang::lexer::Lexer lexer(buffer.str());auto tokens=lexer.tokenize();hyperlang::parser::Parser parser(std::move(tokens));auto program=parser.parse();hyperlang::sema::Analyzer sema;if(!sema.analyze(*program)){for(const auto& d:sema.diagnostics())std::cerr<<d.message<<'\n';return 1;}hyperlang::hil::Module module;for(const auto& f:program->functions){hyperlang::hil::Function hf;hf.name=f->name;hf.instructions.push_back({hyperlang::hil::Opcode::FunctionBegin,f->name});hf.instructions.push_back({hyperlang::hil::Opcode::FunctionEnd,f->name});module.functions.push_back(std::move(hf));}hyperlang::lsc::LiveCompiler compiler;compiler.synchronize(module);std::cout<<"HyperLang compiled revision "<<compiler.snapshot().revision<<"\n";return 0;}
