#include "hyper/LLCVM/LLCVM.h"
#include <unordered_map>
#include <unordered_set>
namespace hyper::llcvm {
class DependencyTracker { std::unordered_map<std::size_t,std::unordered_set<std::size_t>> deps_; public: void add(std::size_t line,std::size_t dependency){deps_[line].insert(dependency);} std::vector<std::size_t> invalidate(std::size_t changed)const{std::vector<std::size_t> out{changed};for(const auto&[line,set]:deps_)if(set.count(changed))out.push_back(line);return out;} };
}