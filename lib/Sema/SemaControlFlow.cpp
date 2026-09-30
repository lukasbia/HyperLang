#include "hyper/Sema/Sema.h"

namespace hyper::sema {

struct ControlFlowFacts {
    bool definitelyReturns = false;
    bool definitelyTerminates = false;
    bool reachable = true;
};

ControlFlowFacts analyzeControlFlow(bool hasReturn, bool hasBreak) {
    ControlFlowFacts facts;
    facts.definitelyReturns = hasReturn;
    facts.definitelyTerminates = hasReturn || hasBreak;
    return facts;
}

} // namespace hyper::sema
