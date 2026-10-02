//===--- PrettyStackTrace.cpp - HyperLang Driver Stack Diagnostics --------===//
#include "hyperlang/Driver/PrettyStackTrace.h"
namespace hyperlang::driver {
PrettyStackTrace::PrettyStackTrace(std::string message) : message_(std::move(message)) {}
const std::string &PrettyStackTrace::message() const { return message_; }
}
