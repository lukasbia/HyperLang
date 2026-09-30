#include "hyper/HIL/HIL.h"
#include <optional>
namespace hyper::hil {
static std::optional<std::int64_t> evaluate(Opcode op,std::int64_t a,std::int64_t b){switch(op){case Opcode::Add:return a+b;case Opcode::Subtract:return a-b;case Opcode::Multiply:return a*b;case Opcode::Divide:return b?a/b:std::nullopt;case Opcode::Remainder:return b?a%b:std::nullopt;default:return std::nullopt;}}
std::optional<std::int64_t> foldArithmetic(Opcode op,std::int64_t a,std::int64_t b){return evaluate(op,a,b);}
}