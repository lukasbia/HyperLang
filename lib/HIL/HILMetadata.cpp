#include "hyper/HIL/HIL.h"
#include <sstream>
namespace hyper::hil {
std::string describeInstruction(const Instruction&i){std::ostringstream out;out<<opcodeName(i.opcode())<<"("<<i.operands().size()<<" operands)";return out.str();}
}