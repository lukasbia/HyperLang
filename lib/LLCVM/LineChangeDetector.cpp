#include "hyper/LLCVM/LLCVM.h"
namespace hyper::llcvm {
bool lineChanged(const Module&m,std::size_t line,std::string_view source){for(const auto&l:m.lines())if(l.sourceLine==line)return l.source!=source;return true;}
}