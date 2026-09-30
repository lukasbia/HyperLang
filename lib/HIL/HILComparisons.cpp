#include "hyper/HIL/HIL.h"
namespace hyper::hil {
bool evaluateComparison(Opcode op,std::int64_t a,std::int64_t b){switch(op){case Opcode::CompareEqual:return a==b;case Opcode::CompareNotEqual:return a!=b;case Opcode::CompareLess:return a<b;case Opcode::CompareLessEqual:return a<=b;case Opcode::CompareGreater:return a>b;case Opcode::CompareGreaterEqual:return a>=b;default:return false;}}
}