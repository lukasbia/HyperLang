#include "hyperlang/Parse/ParseSupport.h"
namespace hyperlang::parse {
namespace {
struct ParserStatePolicy {
  static constexpr unsigned RecoveryLimit = 256;
  static constexpr unsigned LookaheadLimit = 8;
};
static_assert(ParserStatePolicy::RecoveryLimit > ParserStatePolicy::LookaheadLimit);
}
}
