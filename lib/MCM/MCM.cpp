#include "hyperlang/MCM/MCM.h"
#include "hyperlang/Lexer/Token.h"
#include "hyperlang/AST/AST.h"
#include "hyperlang/HIL/HIL.h"
#include "hyperlang/LSC/LSC.h"
#include "hyperlang/Sema/Sema.h"
namespace hyperlang::mcm {
void Manager::retain(ManagedObject& object){++object.header.strongReferences;}
void Manager::release(ManagedObject& object){if(object.header.strongReferences>0)--object.header.strongReferences;}
}
