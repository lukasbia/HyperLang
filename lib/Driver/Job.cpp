//===--- Job.cpp - HyperLang Driver Jobs ---------------------------------===//
#include "hyperlang/Driver/Job.h"
namespace hyperlang::driver {
Job::Job(ActionKind k, std::string t, std::vector<std::string> a, std::string o)
    : kind(k), tool(std::move(t)), arguments(std::move(a)), output(std::move(o)) {}
std::string Job::describe() const {
  std::string result = tool;
  for (const auto &argument : arguments) { result += " "; result += argument; }
  return result;
}
}
