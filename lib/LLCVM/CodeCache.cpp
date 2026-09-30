#include "hyper/LLCVM/LLCVM.h"
#include <unordered_map>
namespace hyper::llcvm {
class LineCache { std::unordered_map<std::size_t,CompiledLine> cache_; public: void put(CompiledLine l){cache_[l.sourceLine]=std::move(l);} const CompiledLine* get(std::size_t n)const{auto i=cache_.find(n);return i==cache_.end()?nullptr:&i->second;} bool contains(std::size_t n)const{return cache_.count(n)!=0;} };
}