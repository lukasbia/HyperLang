#include "hyperlang/MCM/MCM.h"
namespace hyperlang::mcm {
void Manager::retain(ManagedObject& object) {
    ++object.header.strongReferences;
}
void Manager::release(ManagedObject& object) {
    if (object.header.strongReferences > 0) --object.header.strongReferences;
}
void insertOwnershipOperations(hil::Module& module) {
    for (auto& function : module.functions) {
        if (function.instructions.empty()) continue;
        function.instructions.insert(function.instructions.begin() + 1, {hil::Opcode::LoadVariable, "__mcm_scope"});
        function.instructions.insert(function.instructions.end() - 1, {hil::Opcode::StoreVariable, "__mcm_scope"});
    }
}
}
