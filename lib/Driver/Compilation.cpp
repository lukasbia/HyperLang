//===--- Compilation.cpp - HyperLang Compilation Planning -----------------===//
#include "Compilation.h"
namespace hyperlang::driver {
void Compilation::addJob(Job job) { jobs_.push_back(std::move(job)); }
const std::vector<Job> &Compilation::jobs() const { return jobs_; }
bool Compilation::empty() const { return jobs_.empty(); }
void Compilation::clear() { jobs_.clear(); }
}
