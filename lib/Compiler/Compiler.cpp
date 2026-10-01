#include "hyper/Compiler/Compiler.h"
#include "hyper/Sema/Sema.h"
#include <utility>
namespace hyper::compiler {
Compiler::Compiler(Options options):options_(options){}
Result Compiler::compile(std::string_view source,std::string_view fileName){
 Result result;
 Parser parser(source);
 auto syntax=parser.parse();
 for(const auto& d:parser.diagnostics()) if(d.severity==ParseDiagnostic::Severity::Error||d.severity==ParseDiagnostic::Severity::Fatal) result.diagnostics.push_back({Stage::Parse,d.message,true});
 if(!result.diagnostics.empty()) return result;
 result.hilModule=hil::Module(std::string(fileName.empty()?"main":fileName));
 for(std::size_t i=0;i<syntax->children.size();++i){
  const auto& declaration=syntax->children[i];
  hil::Function function(declaration->text.empty()?"main":declaration->text);
  hil::BasicBlock block("entry");
  hil::Instruction constant(hil::Opcode::Constant);
  constant.setResult({static_cast<std::uint32_t>(i+1),"i64",declaration->text});
  block.append(std::move(constant));
  hil::Instruction ret(hil::Opcode::Return);
  if(!declaration->text.empty())ret.addOperand({static_cast<std::uint32_t>(i+1),"i64",declaration->text});
  block.append(std::move(ret));
  function.addBlock(std::move(block)); result.hilModule.addFunction(std::move(function));
 }
 hil::optimization::Pipeline passes;
 if(options_.optimize){ passes.addPass({"constant-folding",true}); passes.addPass({"dead-code-elimination",true}); passes.addPass({"ownership-cleanup",true}); }
 for(const auto& f:result.hilModule.functions()) for(const auto& b:f.blocks()) for(const auto& instruction:b.instructions()){
  llcvm::CompiledLine line; line.sourceLine=result.llcvmModule.lines().size()+1; line.source=hil::opcodeName(instruction.opcode);
  llcvm::Instruction li; li.location.file=std::string(fileName); li.location.line=line.sourceLine;
  switch(instruction.opcode){case hil::Opcode::Constant:li.opcode=llcvm::Opcode::Constant;break;case hil::Opcode::Return:li.opcode=llcvm::Opcode::Return;break;case hil::Opcode::Add:li.opcode=llcvm::Opcode::Add;break;case hil::Opcode::Subtract:li.opcode=llcvm::Opcode::Sub;break;case hil::Opcode::Multiply:li.opcode=llcvm::Opcode::Mul;break;case hil::Opcode::Divide:li.opcode=llcvm::Opcode::Div;break;case hil::Opcode::Retain:li.opcode=llcvm::Opcode::Retain;break;case hil::Opcode::Release:li.opcode=llcvm::Opcode::Release;break;default:li.opcode=llcvm::Opcode::Move;break;}
  line.instructions.push_back(std::move(li)); result.llcvmModule.updateLine(std::move(line));
 }
 result.machineCode=llcvm::backend::Backend(options_.target).emit(result.llcvmModule); result.success=true; return result;
}
} // namespace hyper::compiler
