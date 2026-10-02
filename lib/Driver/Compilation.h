#ifndef HYPERLANG_DRIVER_COMPILATION_H
#define HYPERLANG_DRIVER_COMPILATION_H
#include "hyperlang/Driver/Job.h"
#include <utility>
#include <vector>
namespace hyperlang::driver {
class Compilation {
public:
  void addJob(Job job);
  const std::vector<Job> &jobs() const;
  bool empty() const;
  void clear();
private:
  std::vector<Job> jobs_;
};
}
#endif
