#include "hyperlang/LSC/LSC.h"
#include "hyperlang/Lexer/Token.h"
#include "hyperlang/AST/AST.h"
#include "hyperlang/HIL/HIL.h"
#include "hyperlang/MCM/MCM.h"
#include "hyperlang/Sema/Sema.h"
namespace hyperlang::lsc {
void LiveCompiler::synchronize(const hil::Module& module){snapshot_.module=module;++snapshot_.revision;if(callback_)callback_(snapshot_);}
void LiveCompiler::setUpdateCallback(UpdateCallback callback){callback_=std::move(callback);}
const CompilationSnapshot& LiveCompiler::snapshot()const{return snapshot_;}
}
