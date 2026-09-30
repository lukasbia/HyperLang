#include "hyper/LLCVM/LLCVM.h"
#include <algorithm>

namespace hyper::llcvm {

void IncrementalScheduler::schedule(std::size_t line) {
    if (line == 0) return;
    if (std::find(pending_.begin(), pending_.end(), line) == pending_.end()) pending_.push_back(line);
    std::sort(pending_.begin(), pending_.end());
}

bool IncrementalScheduler::empty() const noexcept { return pending_.empty(); }

std::size_t IncrementalScheduler::next() {
    if (pending_.empty()) return 0;
    const auto line = pending_.front();
    pending_.erase(pending_.begin());
    return line;
}

} // namespace hyper::llcvm
