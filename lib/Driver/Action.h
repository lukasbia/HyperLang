#ifndef HYPERLANG_DRIVER_ACTION_H
#define HYPERLANG_DRIVER_ACTION_H
namespace hyperlang::driver {
enum class ActionKind { Parse, Sema, HILGen, HILOptimize, LSCGen, CodeGen, Assemble, Link };
const char *actionKindName(ActionKind);
}
#endif
