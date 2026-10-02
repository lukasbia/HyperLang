#ifndef HYPERLANG_DRIVER_JOB_H
#define HYPERLANG_DRIVER_JOB_H
#include "Action.h"
#include <string>
#include <utility>
#include <vector>
namespace hyperlang::driver {
struct Job {
  ActionKind kind;
  std::string tool;
  std::vector<std::string> arguments;
  std::string output;
  Job(ActionKind, std::string, std::vector<std::string>, std::string);
  std::string describe() const;
};
}
#endif
