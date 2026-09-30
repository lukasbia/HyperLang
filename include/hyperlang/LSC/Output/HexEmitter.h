#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace hyperlang::lsc::output {
struct HexByte { std::uint8_t value{}; };
struct HexImage {
    std::string targetTriple;
    std::vector<HexByte> bytes;
    std::string text() const;
};
class HexEmitter {
public:
    HexImage emit(const std::vector<std::uint8_t>& machineCode, std::string targetTriple) const;
    static std::string formatByte(std::uint8_t value);
    static std::string formatImage(const std::vector<std::uint8_t>& bytes);
};
}
