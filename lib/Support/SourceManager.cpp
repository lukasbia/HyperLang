#include "hyperlang/Support/SourceManager.h"
#include "hyperlang/Lexer/Token.h"
#include "hyperlang/AST/AST.h"
#include "hyperlang/HIL/HIL.h"
#include "hyperlang/MCM/MCM.h"
#include "hyperlang/LSC/LSC.h"
namespace hyperlang::support {
bool SourceManager::addSource(std::string path,std::string source){return sources_.emplace(std::move(path),std::move(source)).second;}
std::string_view SourceManager::source(std::string_view path)const{auto it=sources_.find(std::string(path));return it==sources_.end()?std::string_view{}:it->second;}
}
