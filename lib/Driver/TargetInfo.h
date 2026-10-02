#ifndef HYPERLANG_DRIVER_TARGET_INFO_H
#define HYPERLANG_DRIVER_TARGET_INFO_H
#include <string>
namespace hyperlang::driver {
struct TargetInfo {
  std::string architecture = "x86_64";
  std::string vendor = "unknown";
  std::string operatingSystem = "linux";
  std::string environment = "gnu";
  std::string platform = "linux";
  std::string triple() const {
    return architecture + "-" + vendor + "-" + operatingSystem + "-" + environment;
  }
};
}
#endif
