#include "hyper/LLCVM/LLCVM.h"

namespace hyper::llcvm {

void SourceTracker::replace(SourceSnapshot snapshot) {
    snapshot_ = std::move(snapshot);
}

const SourceSnapshot& SourceTracker::snapshot() const noexcept {
    return snapshot_;
}

std::vector<std::size_t> SourceTracker::changedLines(const SourceSnapshot& next) const {
    return LineChangeDetector::detect(snapshot_, next);
}

} // namespace hyper::llcvm
