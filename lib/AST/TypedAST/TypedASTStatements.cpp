#include "hyper/AST/TypedAST/TypedAST.h"
namespace hyper::typedast {
bool statementIsTerminating(const Statement& statement) noexcept { return statement.terminating; }
bool statementIsReachable(const Statement& statement) noexcept { return statement.reachable; }
}
