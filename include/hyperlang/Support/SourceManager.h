#pragma once
#include <string>
#include <string_view>
#include <unordered_map>
namespace hyperlang::support {
class SourceManager {
public: bool addSource(std::string path, std::string source); std::string_view source(std::string_view path) const;
private: std::unordered_map<std::string,std::string> sources_;
};
}
