#include "hyper/HIL/HIL.h"
namespace hyper::hil {
Value makeValue(std::uint32_t id,const std::string&type,const std::string&name){return Value{id,type,name};}
bool sameValue(const Value&a,const Value&b){return a.id==b.id&&a.type==b.type;}
}