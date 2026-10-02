#ifndef HYPERLANG_DRIVER_FRONTEND_UTIL_H
#define HYPERLANG_DRIVER_FRONTEND_UTIL_H
#include <string>
#include <string_view>
namespace hyperlang::driver {
struct FrontendUtil {
  static std::string stem(std::string_view path);
  static bool isSourceFile(std::string_view path);
};
}
#endif
