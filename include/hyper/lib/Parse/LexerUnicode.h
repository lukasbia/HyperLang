#pragma once
#ifndef HYPER_LIB_PARSE_LEXER_UNICODE_H
#define HYPER_LIB_PARSE_LEXER_UNICODE_H
#include <cstddef>
#include <cstdint>
namespace hyper::parse {
struct LexerUnicode {
    static bool isUTF8LeadByte(unsigned char value) noexcept {
        return value < 0x80 || (value >= 0xC2 && value < 0xF5);
    }
};
}
#endif
