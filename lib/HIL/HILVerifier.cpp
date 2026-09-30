#include "hyper/HIL/HIL.h"
#include <string>
namespace hyper::hil {
bool verify(const Module&m,std::string*error){for(const auto&f:m.functions())for(const auto&b:f.blocks()){if(b.instructions().empty()){if(error)*error="empty basic block: "+b.name();return false;}if(!isTerminator(b.instructions().back().opcode())){if(error)*error="unterminated basic block: "+b.name();return false;}for(const auto&i:b.instructions())if(i.opcode()==Opcode::Call&&i.operands().empty()){if(error)*error="call without callee";return false;}}return true;}
}