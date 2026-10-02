//===--- FrontendUtil.cpp - HyperLang Frontend Driver Utilities -----------===//
#include "hyperlang/Driver/FrontendUtil.h"
#include <filesystem>
namespace hyperlang::driver {
std::string FrontendUtil::stem(std::string_view path) {
  return std::filesystem::path(path).stem().string();
}
bool FrontendUtil::isSourceFile(std::string_view path) {
  return std::filesystem::path(path).extension() == ".hyper";
}
}
