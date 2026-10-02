#ifndef HYPERLANG_DRIVER_PRETTY_STACK_TRACE_H
#define HYPERLANG_DRIVER_PRETTY_STACK_TRACE_H
#include <string>
#include <utility>
namespace hyperlang::driver {
class PrettyStackTrace {
public:
  explicit PrettyStackTrace(std::string message);
  const std::string &message() const;
private:
  std::string message_;
};
}
#endif
