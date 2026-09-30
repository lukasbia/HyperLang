#include "hyper/LLCVM/LLCVM.h"
#include "hyper/LLCVM/IRGenerator.h"

namespace hyper::llcvm {

std::vector<std::size_t> compileChangedLines(Module& module,
                                              SourceTracker& tracker,
                                              const SourceSnapshot& next) {
    const auto changed = tracker.changedLines(next);
    IRGenerator generator;
    for (const auto line : changed) {
        if (line == 0 || line > next.lines.size()) continue;
        auto compiled = generator.generateLine(next.lines[line - 1], line);
        std::string error;
        if (verifyLine(compiled, &error)) module.updateLine(std::move(compiled));
    }
    for (std::size_t oldLine = next.lines.size() + 1; oldLine <= tracker.snapshot().lines.size(); ++oldLine) {
        module.removeLine(oldLine);
    }
    tracker.replace(next);
    return changed;
}

} // namespace hyper::llcvm
