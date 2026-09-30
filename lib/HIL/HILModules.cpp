#include "hyper/HIL/HIL.h"
namespace hyper::hil {
Function* findFunction(Module&m,const std::string&name){for(auto&f:m.functions())if(f.name()==name)return &f;return nullptr;}
bool hasFunction(const Module&m,const std::string&name){for(const auto&f:m.functions())if(f.name()==name)return true;return false;}
}