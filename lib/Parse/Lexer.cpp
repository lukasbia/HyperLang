//===--- Lexer.cpp - HyperLang Source Lexer -------------------------------===//

#include "hyperlang/Parse/Lexer.h"
#include "hyperlang/Parse/LexerDiagnostics.h"
#include "hyperlang/Parse/LexerKeywords.h"
#include "hyperlang/Parse/LexerLiterals.h"
#include "hyperlang/Parse/LexerOperators.h"
#include "hyperlang/Parse/LexerOptions.h"
#include "hyperlang/Parse/LexerToken.h"
#include "hyperlang/Parse/LexerTrivia.h"
#include "hyperlang/Parse/LexerUnicode.h"
#include "hyperlang/Basic/UTF8.h"

#include <algorithm>
#include <array>
#include <charconv>
#include <cerrno>
#include <cstdlib>
#include <limits>
#include <string>
#include <string_view>

namespace hyperlang {
namespace {

std::string hexByte(unsigned char value) {
  static constexpr char digits[] = "0123456789ABCDEF";
  std::string result = "0x";
  result += digits[value >> 4];
  result += digits[value & 0x0F];
  return result;
}


// ASCII identifier-start classification.
static constexpr std::array<unsigned char, 256> kASCIIIdentifierStart = {{
  0, // 0x00
  0, // 0x01
  0, // 0x02
  0, // 0x03
  0, // 0x04
  0, // 0x05
  0, // 0x06
  0, // 0x07
  0, // 0x08
  0, // 0x09
  0, // 0x0A
  0, // 0x0B
  0, // 0x0C
  0, // 0x0D
  0, // 0x0E
  0, // 0x0F
  0, // 0x10
  0, // 0x11
  0, // 0x12
  0, // 0x13
  0, // 0x14
  0, // 0x15
  0, // 0x16
  0, // 0x17
  0, // 0x18
  0, // 0x19
  0, // 0x1A
  0, // 0x1B
  0, // 0x1C
  0, // 0x1D
  0, // 0x1E
  0, // 0x1F
  0, // 0x20
  0, // 0x21
  0, // 0x22
  0, // 0x23
  0, // 0x24
  0, // 0x25
  0, // 0x26
  0, // 0x27
  0, // 0x28
  0, // 0x29
  0, // 0x2A
  0, // 0x2B
  0, // 0x2C
  0, // 0x2D
  0, // 0x2E
  0, // 0x2F
  0, // 0x30
  0, // 0x31
  0, // 0x32
  0, // 0x33
  0, // 0x34
  0, // 0x35
  0, // 0x36
  0, // 0x37
  0, // 0x38
  0, // 0x39
  0, // 0x3A
  0, // 0x3B
  0, // 0x3C
  0, // 0x3D
  0, // 0x3E
  0, // 0x3F
  0, // 0x40
  1, // 0x41
  1, // 0x42
  1, // 0x43
  1, // 0x44
  1, // 0x45
  1, // 0x46
  1, // 0x47
  1, // 0x48
  1, // 0x49
  1, // 0x4A
  1, // 0x4B
  1, // 0x4C
  1, // 0x4D
  1, // 0x4E
  1, // 0x4F
  1, // 0x50
  1, // 0x51
  1, // 0x52
  1, // 0x53
  1, // 0x54
  1, // 0x55
  1, // 0x56
  1, // 0x57
  1, // 0x58
  1, // 0x59
  1, // 0x5A
  0, // 0x5B
  0, // 0x5C
  0, // 0x5D
  0, // 0x5E
  1, // 0x5F
  0, // 0x60
  1, // 0x61
  1, // 0x62
  1, // 0x63
  1, // 0x64
  1, // 0x65
  1, // 0x66
  1, // 0x67
  1, // 0x68
  1, // 0x69
  1, // 0x6A
  1, // 0x6B
  1, // 0x6C
  1, // 0x6D
  1, // 0x6E
  1, // 0x6F
  1, // 0x70
  1, // 0x71
  1, // 0x72
  1, // 0x73
  1, // 0x74
  1, // 0x75
  1, // 0x76
  1, // 0x77
  1, // 0x78
  1, // 0x79
  1, // 0x7A
  0, // 0x7B
  0, // 0x7C
  0, // 0x7D
  0, // 0x7E
  0, // 0x7F
  0, // 0x80
  0, // 0x81
  0, // 0x82
  0, // 0x83
  0, // 0x84
  0, // 0x85
  0, // 0x86
  0, // 0x87
  0, // 0x88
  0, // 0x89
  0, // 0x8A
  0, // 0x8B
  0, // 0x8C
  0, // 0x8D
  0, // 0x8E
  0, // 0x8F
  0, // 0x90
  0, // 0x91
  0, // 0x92
  0, // 0x93
  0, // 0x94
  0, // 0x95
  0, // 0x96
  0, // 0x97
  0, // 0x98
  0, // 0x99
  0, // 0x9A
  0, // 0x9B
  0, // 0x9C
  0, // 0x9D
  0, // 0x9E
  0, // 0x9F
  0, // 0xA0
  0, // 0xA1
  0, // 0xA2
  0, // 0xA3
  0, // 0xA4
  0, // 0xA5
  0, // 0xA6
  0, // 0xA7
  0, // 0xA8
  0, // 0xA9
  0, // 0xAA
  0, // 0xAB
  0, // 0xAC
  0, // 0xAD
  0, // 0xAE
  0, // 0xAF
  0, // 0xB0
  0, // 0xB1
  0, // 0xB2
  0, // 0xB3
  0, // 0xB4
  0, // 0xB5
  0, // 0xB6
  0, // 0xB7
  0, // 0xB8
  0, // 0xB9
  0, // 0xBA
  0, // 0xBB
  0, // 0xBC
  0, // 0xBD
  0, // 0xBE
  0, // 0xBF
  0, // 0xC0
  0, // 0xC1
  0, // 0xC2
  0, // 0xC3
  0, // 0xC4
  0, // 0xC5
  0, // 0xC6
  0, // 0xC7
  0, // 0xC8
  0, // 0xC9
  0, // 0xCA
  0, // 0xCB
  0, // 0xCC
  0, // 0xCD
  0, // 0xCE
  0, // 0xCF
  0, // 0xD0
  0, // 0xD1
  0, // 0xD2
  0, // 0xD3
  0, // 0xD4
  0, // 0xD5
  0, // 0xD6
  0, // 0xD7
  0, // 0xD8
  0, // 0xD9
  0, // 0xDA
  0, // 0xDB
  0, // 0xDC
  0, // 0xDD
  0, // 0xDE
  0, // 0xDF
  0, // 0xE0
  0, // 0xE1
  0, // 0xE2
  0, // 0xE3
  0, // 0xE4
  0, // 0xE5
  0, // 0xE6
  0, // 0xE7
  0, // 0xE8
  0, // 0xE9
  0, // 0xEA
  0, // 0xEB
  0, // 0xEC
  0, // 0xED
  0, // 0xEE
  0, // 0xEF
  0, // 0xF0
  0, // 0xF1
  0, // 0xF2
  0, // 0xF3
  0, // 0xF4
  0, // 0xF5
  0, // 0xF6
  0, // 0xF7
  0, // 0xF8
  0, // 0xF9
  0, // 0xFA
  0, // 0xFB
  0, // 0xFC
  0, // 0xFD
  0, // 0xFE
  0, // 0xFF
}};

// ASCII identifier-continue classification.
static constexpr std::array<unsigned char, 256> kASCIIIdentifierContinue = {{
  0, // 0x00
  0, // 0x01
  0, // 0x02
  0, // 0x03
  0, // 0x04
  0, // 0x05
  0, // 0x06
  0, // 0x07
  0, // 0x08
  0, // 0x09
  0, // 0x0A
  0, // 0x0B
  0, // 0x0C
  0, // 0x0D
  0, // 0x0E
  0, // 0x0F
  0, // 0x10
  0, // 0x11
  0, // 0x12
  0, // 0x13
  0, // 0x14
  0, // 0x15
  0, // 0x16
  0, // 0x17
  0, // 0x18
  0, // 0x19
  0, // 0x1A
  0, // 0x1B
  0, // 0x1C
  0, // 0x1D
  0, // 0x1E
  0, // 0x1F
  0, // 0x20
  0, // 0x21
  0, // 0x22
  0, // 0x23
  0, // 0x24
  0, // 0x25
  0, // 0x26
  0, // 0x27
  0, // 0x28
  0, // 0x29
  0, // 0x2A
  0, // 0x2B
  0, // 0x2C
  0, // 0x2D
  0, // 0x2E
  0, // 0x2F
  1, // 0x30
  1, // 0x31
  1, // 0x32
  1, // 0x33
  1, // 0x34
  1, // 0x35
  1, // 0x36
  1, // 0x37
  1, // 0x38
  1, // 0x39
  0, // 0x3A
  0, // 0x3B
  0, // 0x3C
  0, // 0x3D
  0, // 0x3E
  0, // 0x3F
  0, // 0x40
  1, // 0x41
  1, // 0x42
  1, // 0x43
  1, // 0x44
  1, // 0x45
  1, // 0x46
  1, // 0x47
  1, // 0x48
  1, // 0x49
  1, // 0x4A
  1, // 0x4B
  1, // 0x4C
  1, // 0x4D
  1, // 0x4E
  1, // 0x4F
  1, // 0x50
  1, // 0x51
  1, // 0x52
  1, // 0x53
  1, // 0x54
  1, // 0x55
  1, // 0x56
  1, // 0x57
  1, // 0x58
  1, // 0x59
  1, // 0x5A
  0, // 0x5B
  0, // 0x5C
  0, // 0x5D
  0, // 0x5E
  1, // 0x5F
  0, // 0x60
  1, // 0x61
  1, // 0x62
  1, // 0x63
  1, // 0x64
  1, // 0x65
  1, // 0x66
  1, // 0x67
  1, // 0x68
  1, // 0x69
  1, // 0x6A
  1, // 0x6B
  1, // 0x6C
  1, // 0x6D
  1, // 0x6E
  1, // 0x6F
  1, // 0x70
  1, // 0x71
  1, // 0x72
  1, // 0x73
  1, // 0x74
  1, // 0x75
  1, // 0x76
  1, // 0x77
  1, // 0x78
  1, // 0x79
  1, // 0x7A
  0, // 0x7B
  0, // 0x7C
  0, // 0x7D
  0, // 0x7E
  0, // 0x7F
  0, // 0x80
  0, // 0x81
  0, // 0x82
  0, // 0x83
  0, // 0x84
  0, // 0x85
  0, // 0x86
  0, // 0x87
  0, // 0x88
  0, // 0x89
  0, // 0x8A
  0, // 0x8B
  0, // 0x8C
  0, // 0x8D
  0, // 0x8E
  0, // 0x8F
  0, // 0x90
  0, // 0x91
  0, // 0x92
  0, // 0x93
  0, // 0x94
  0, // 0x95
  0, // 0x96
  0, // 0x97
  0, // 0x98
  0, // 0x99
  0, // 0x9A
  0, // 0x9B
  0, // 0x9C
  0, // 0x9D
  0, // 0x9E
  0, // 0x9F
  0, // 0xA0
  0, // 0xA1
  0, // 0xA2
  0, // 0xA3
  0, // 0xA4
  0, // 0xA5
  0, // 0xA6
  0, // 0xA7
  0, // 0xA8
  0, // 0xA9
  0, // 0xAA
  0, // 0xAB
  0, // 0xAC
  0, // 0xAD
  0, // 0xAE
  0, // 0xAF
  0, // 0xB0
  0, // 0xB1
  0, // 0xB2
  0, // 0xB3
  0, // 0xB4
  0, // 0xB5
  0, // 0xB6
  0, // 0xB7
  0, // 0xB8
  0, // 0xB9
  0, // 0xBA
  0, // 0xBB
  0, // 0xBC
  0, // 0xBD
  0, // 0xBE
  0, // 0xBF
  0, // 0xC0
  0, // 0xC1
  0, // 0xC2
  0, // 0xC3
  0, // 0xC4
  0, // 0xC5
  0, // 0xC6
  0, // 0xC7
  0, // 0xC8
  0, // 0xC9
  0, // 0xCA
  0, // 0xCB
  0, // 0xCC
  0, // 0xCD
  0, // 0xCE
  0, // 0xCF
  0, // 0xD0
  0, // 0xD1
  0, // 0xD2
  0, // 0xD3
  0, // 0xD4
  0, // 0xD5
  0, // 0xD6
  0, // 0xD7
  0, // 0xD8
  0, // 0xD9
  0, // 0xDA
  0, // 0xDB
  0, // 0xDC
  0, // 0xDD
  0, // 0xDE
  0, // 0xDF
  0, // 0xE0
  0, // 0xE1
  0, // 0xE2
  0, // 0xE3
  0, // 0xE4
  0, // 0xE5
  0, // 0xE6
  0, // 0xE7
  0, // 0xE8
  0, // 0xE9
  0, // 0xEA
  0, // 0xEB
  0, // 0xEC
  0, // 0xED
  0, // 0xEE
  0, // 0xEF
  0, // 0xF0
  0, // 0xF1
  0, // 0xF2
  0, // 0xF3
  0, // 0xF4
  0, // 0xF5
  0, // 0xF6
  0, // 0xF7
  0, // 0xF8
  0, // 0xF9
  0, // 0xFA
  0, // 0xFB
  0, // 0xFC
  0, // 0xFD
  0, // 0xFE
  0, // 0xFF
}};

// ASCII digit and hexadecimal-digit values. 255 means invalid.
static constexpr std::array<unsigned char, 256> kASCIIDigitValue = {{
  255, // 0x00
  255, // 0x01
  255, // 0x02
  255, // 0x03
  255, // 0x04
  255, // 0x05
  255, // 0x06
  255, // 0x07
  255, // 0x08
  255, // 0x09
  255, // 0x0A
  255, // 0x0B
  255, // 0x0C
  255, // 0x0D
  255, // 0x0E
  255, // 0x0F
  255, // 0x10
  255, // 0x11
  255, // 0x12
  255, // 0x13
  255, // 0x14
  255, // 0x15
  255, // 0x16
  255, // 0x17
  255, // 0x18
  255, // 0x19
  255, // 0x1A
  255, // 0x1B
  255, // 0x1C
  255, // 0x1D
  255, // 0x1E
  255, // 0x1F
  255, // 0x20
  255, // 0x21
  255, // 0x22
  255, // 0x23
  255, // 0x24
  255, // 0x25
  255, // 0x26
  255, // 0x27
  255, // 0x28
  255, // 0x29
  255, // 0x2A
  255, // 0x2B
  255, // 0x2C
  255, // 0x2D
  255, // 0x2E
  255, // 0x2F
  0, // 0x30
  1, // 0x31
  2, // 0x32
  3, // 0x33
  4, // 0x34
  5, // 0x35
  6, // 0x36
  7, // 0x37
  8, // 0x38
  9, // 0x39
  255, // 0x3A
  255, // 0x3B
  255, // 0x3C
  255, // 0x3D
  255, // 0x3E
  255, // 0x3F
  255, // 0x40
  10, // 0x41
  11, // 0x42
  12, // 0x43
  13, // 0x44
  14, // 0x45
  15, // 0x46
  255, // 0x47
  255, // 0x48
  255, // 0x49
  255, // 0x4A
  255, // 0x4B
  255, // 0x4C
  255, // 0x4D
  255, // 0x4E
  255, // 0x4F
  255, // 0x50
  255, // 0x51
  255, // 0x52
  255, // 0x53
  255, // 0x54
  255, // 0x55
  255, // 0x56
  255, // 0x57
  255, // 0x58
  255, // 0x59
  255, // 0x5A
  255, // 0x5B
  255, // 0x5C
  255, // 0x5D
  255, // 0x5E
  255, // 0x5F
  255, // 0x60
  10, // 0x61
  11, // 0x62
  12, // 0x63
  13, // 0x64
  14, // 0x65
  15, // 0x66
  255, // 0x67
  255, // 0x68
  255, // 0x69
  255, // 0x6A
  255, // 0x6B
  255, // 0x6C
  255, // 0x6D
  255, // 0x6E
  255, // 0x6F
  255, // 0x70
  255, // 0x71
  255, // 0x72
  255, // 0x73
  255, // 0x74
  255, // 0x75
  255, // 0x76
  255, // 0x77
  255, // 0x78
  255, // 0x79
  255, // 0x7A
  255, // 0x7B
  255, // 0x7C
  255, // 0x7D
  255, // 0x7E
  255, // 0x7F
  255, // 0x80
  255, // 0x81
  255, // 0x82
  255, // 0x83
  255, // 0x84
  255, // 0x85
  255, // 0x86
  255, // 0x87
  255, // 0x88
  255, // 0x89
  255, // 0x8A
  255, // 0x8B
  255, // 0x8C
  255, // 0x8D
  255, // 0x8E
  255, // 0x8F
  255, // 0x90
  255, // 0x91
  255, // 0x92
  255, // 0x93
  255, // 0x94
  255, // 0x95
  255, // 0x96
  255, // 0x97
  255, // 0x98
  255, // 0x99
  255, // 0x9A
  255, // 0x9B
  255, // 0x9C
  255, // 0x9D
  255, // 0x9E
  255, // 0x9F
  255, // 0xA0
  255, // 0xA1
  255, // 0xA2
  255, // 0xA3
  255, // 0xA4
  255, // 0xA5
  255, // 0xA6
  255, // 0xA7
  255, // 0xA8
  255, // 0xA9
  255, // 0xAA
  255, // 0xAB
  255, // 0xAC
  255, // 0xAD
  255, // 0xAE
  255, // 0xAF
  255, // 0xB0
  255, // 0xB1
  255, // 0xB2
  255, // 0xB3
  255, // 0xB4
  255, // 0xB5
  255, // 0xB6
  255, // 0xB7
  255, // 0xB8
  255, // 0xB9
  255, // 0xBA
  255, // 0xBB
  255, // 0xBC
  255, // 0xBD
  255, // 0xBE
  255, // 0xBF
  255, // 0xC0
  255, // 0xC1
  255, // 0xC2
  255, // 0xC3
  255, // 0xC4
  255, // 0xC5
  255, // 0xC6
  255, // 0xC7
  255, // 0xC8
  255, // 0xC9
  255, // 0xCA
  255, // 0xCB
  255, // 0xCC
  255, // 0xCD
  255, // 0xCE
  255, // 0xCF
  255, // 0xD0
  255, // 0xD1
  255, // 0xD2
  255, // 0xD3
  255, // 0xD4
  255, // 0xD5
  255, // 0xD6
  255, // 0xD7
  255, // 0xD8
  255, // 0xD9
  255, // 0xDA
  255, // 0xDB
  255, // 0xDC
  255, // 0xDD
  255, // 0xDE
  255, // 0xDF
  255, // 0xE0
  255, // 0xE1
  255, // 0xE2
  255, // 0xE3
  255, // 0xE4
  255, // 0xE5
  255, // 0xE6
  255, // 0xE7
  255, // 0xE8
  255, // 0xE9
  255, // 0xEA
  255, // 0xEB
  255, // 0xEC
  255, // 0xED
  255, // 0xEE
  255, // 0xEF
  255, // 0xF0
  255, // 0xF1
  255, // 0xF2
  255, // 0xF3
  255, // 0xF4
  255, // 0xF5
  255, // 0xF6
  255, // 0xF7
  255, // 0xF8
  255, // 0xF9
  255, // 0xFA
  255, // 0xFB
  255, // 0xFC
  255, // 0xFD
  255, // 0xFE
  255, // 0xFF
}};

// ASCII whitespace classification used by trivia scanning.
static constexpr std::array<unsigned char, 256> kASCIIWhitespace = {{
  0, // 0x00
  0, // 0x01
  0, // 0x02
  0, // 0x03
  0, // 0x04
  0, // 0x05
  0, // 0x06
  0, // 0x07
  0, // 0x08
  1, // 0x09
  1, // 0x0A
  1, // 0x0B
  1, // 0x0C
  1, // 0x0D
  0, // 0x0E
  0, // 0x0F
  0, // 0x10
  0, // 0x11
  0, // 0x12
  0, // 0x13
  0, // 0x14
  0, // 0x15
  0, // 0x16
  0, // 0x17
  0, // 0x18
  0, // 0x19
  0, // 0x1A
  0, // 0x1B
  0, // 0x1C
  0, // 0x1D
  0, // 0x1E
  0, // 0x1F
  1, // 0x20
  0, // 0x21
  0, // 0x22
  0, // 0x23
  0, // 0x24
  0, // 0x25
  0, // 0x26
  0, // 0x27
  0, // 0x28
  0, // 0x29
  0, // 0x2A
  0, // 0x2B
  0, // 0x2C
  0, // 0x2D
  0, // 0x2E
  0, // 0x2F
  0, // 0x30
  0, // 0x31
  0, // 0x32
  0, // 0x33
  0, // 0x34
  0, // 0x35
  0, // 0x36
  0, // 0x37
  0, // 0x38
  0, // 0x39
  0, // 0x3A
  0, // 0x3B
  0, // 0x3C
  0, // 0x3D
  0, // 0x3E
  0, // 0x3F
  0, // 0x40
  0, // 0x41
  0, // 0x42
  0, // 0x43
  0, // 0x44
  0, // 0x45
  0, // 0x46
  0, // 0x47
  0, // 0x48
  0, // 0x49
  0, // 0x4A
  0, // 0x4B
  0, // 0x4C
  0, // 0x4D
  0, // 0x4E
  0, // 0x4F
  0, // 0x50
  0, // 0x51
  0, // 0x52
  0, // 0x53
  0, // 0x54
  0, // 0x55
  0, // 0x56
  0, // 0x57
  0, // 0x58
  0, // 0x59
  0, // 0x5A
  0, // 0x5B
  0, // 0x5C
  0, // 0x5D
  0, // 0x5E
  0, // 0x5F
  0, // 0x60
  0, // 0x61
  0, // 0x62
  0, // 0x63
  0, // 0x64
  0, // 0x65
  0, // 0x66
  0, // 0x67
  0, // 0x68
  0, // 0x69
  0, // 0x6A
  0, // 0x6B
  0, // 0x6C
  0, // 0x6D
  0, // 0x6E
  0, // 0x6F
  0, // 0x70
  0, // 0x71
  0, // 0x72
  0, // 0x73
  0, // 0x74
  0, // 0x75
  0, // 0x76
  0, // 0x77
  0, // 0x78
  0, // 0x79
  0, // 0x7A
  0, // 0x7B
  0, // 0x7C
  0, // 0x7D
  0, // 0x7E
  0, // 0x7F
  0, // 0x80
  0, // 0x81
  0, // 0x82
  0, // 0x83
  0, // 0x84
  0, // 0x85
  0, // 0x86
  0, // 0x87
  0, // 0x88
  0, // 0x89
  0, // 0x8A
  0, // 0x8B
  0, // 0x8C
  0, // 0x8D
  0, // 0x8E
  0, // 0x8F
  0, // 0x90
  0, // 0x91
  0, // 0x92
  0, // 0x93
  0, // 0x94
  0, // 0x95
  0, // 0x96
  0, // 0x97
  0, // 0x98
  0, // 0x99
  0, // 0x9A
  0, // 0x9B
  0, // 0x9C
  0, // 0x9D
  0, // 0x9E
  0, // 0x9F
  0, // 0xA0
  0, // 0xA1
  0, // 0xA2
  0, // 0xA3
  0, // 0xA4
  0, // 0xA5
  0, // 0xA6
  0, // 0xA7
  0, // 0xA8
  0, // 0xA9
  0, // 0xAA
  0, // 0xAB
  0, // 0xAC
  0, // 0xAD
  0, // 0xAE
  0, // 0xAF
  0, // 0xB0
  0, // 0xB1
  0, // 0xB2
  0, // 0xB3
  0, // 0xB4
  0, // 0xB5
  0, // 0xB6
  0, // 0xB7
  0, // 0xB8
  0, // 0xB9
  0, // 0xBA
  0, // 0xBB
  0, // 0xBC
  0, // 0xBD
  0, // 0xBE
  0, // 0xBF
  0, // 0xC0
  0, // 0xC1
  0, // 0xC2
  0, // 0xC3
  0, // 0xC4
  0, // 0xC5
  0, // 0xC6
  0, // 0xC7
  0, // 0xC8
  0, // 0xC9
  0, // 0xCA
  0, // 0xCB
  0, // 0xCC
  0, // 0xCD
  0, // 0xCE
  0, // 0xCF
  0, // 0xD0
  0, // 0xD1
  0, // 0xD2
  0, // 0xD3
  0, // 0xD4
  0, // 0xD5
  0, // 0xD6
  0, // 0xD7
  0, // 0xD8
  0, // 0xD9
  0, // 0xDA
  0, // 0xDB
  0, // 0xDC
  0, // 0xDD
  0, // 0xDE
  0, // 0xDF
  0, // 0xE0
  0, // 0xE1
  0, // 0xE2
  0, // 0xE3
  0, // 0xE4
  0, // 0xE5
  0, // 0xE6
  0, // 0xE7
  0, // 0xE8
  0, // 0xE9
  0, // 0xEA
  0, // 0xEB
  0, // 0xEC
  0, // 0xED
  0, // 0xEE
  0, // 0xEF
  0, // 0xF0
  0, // 0xF1
  0, // 0xF2
  0, // 0xF3
  0, // 0xF4
  0, // 0xF5
  0, // 0xF6
  0, // 0xF7
  0, // 0xF8
  0, // 0xF9
  0, // 0xFA
  0, // 0xFB
  0, // 0xFC
  0, // 0xFD
  0, // 0xFE
  0, // 0xFF
}};

// ASCII operator-character classification.
static constexpr std::array<unsigned char, 256> kASCIIOperatorCharacter = {{
  0, // 0x00
  0, // 0x01
  0, // 0x02
  0, // 0x03
  0, // 0x04
  0, // 0x05
  0, // 0x06
  0, // 0x07
  0, // 0x08
  0, // 0x09
  0, // 0x0A
  0, // 0x0B
  0, // 0x0C
  0, // 0x0D
  0, // 0x0E
  0, // 0x0F
  0, // 0x10
  0, // 0x11
  0, // 0x12
  0, // 0x13
  0, // 0x14
  0, // 0x15
  0, // 0x16
  0, // 0x17
  0, // 0x18
  0, // 0x19
  0, // 0x1A
  0, // 0x1B
  0, // 0x1C
  0, // 0x1D
  0, // 0x1E
  0, // 0x1F
  0, // 0x20
  1, // 0x21
  0, // 0x22
  0, // 0x23
  0, // 0x24
  1, // 0x25
  1, // 0x26
  0, // 0x27
  0, // 0x28
  0, // 0x29
  1, // 0x2A
  1, // 0x2B
  0, // 0x2C
  1, // 0x2D
  1, // 0x2E
  1, // 0x2F
  0, // 0x30
  0, // 0x31
  0, // 0x32
  0, // 0x33
  0, // 0x34
  0, // 0x35
  0, // 0x36
  0, // 0x37
  0, // 0x38
  0, // 0x39
  1, // 0x3A
  0, // 0x3B
  1, // 0x3C
  1, // 0x3D
  1, // 0x3E
  1, // 0x3F
  0, // 0x40
  0, // 0x41
  0, // 0x42
  0, // 0x43
  0, // 0x44
  0, // 0x45
  0, // 0x46
  0, // 0x47
  0, // 0x48
  0, // 0x49
  0, // 0x4A
  0, // 0x4B
  0, // 0x4C
  0, // 0x4D
  0, // 0x4E
  0, // 0x4F
  0, // 0x50
  0, // 0x51
  0, // 0x52
  0, // 0x53
  0, // 0x54
  0, // 0x55
  0, // 0x56
  0, // 0x57
  0, // 0x58
  0, // 0x59
  0, // 0x5A
  0, // 0x5B
  0, // 0x5C
  0, // 0x5D
  1, // 0x5E
  0, // 0x5F
  0, // 0x60
  0, // 0x61
  0, // 0x62
  0, // 0x63
  0, // 0x64
  0, // 0x65
  0, // 0x66
  0, // 0x67
  0, // 0x68
  0, // 0x69
  0, // 0x6A
  0, // 0x6B
  0, // 0x6C
  0, // 0x6D
  0, // 0x6E
  0, // 0x6F
  0, // 0x70
  0, // 0x71
  0, // 0x72
  0, // 0x73
  0, // 0x74
  0, // 0x75
  0, // 0x76
  0, // 0x77
  0, // 0x78
  0, // 0x79
  0, // 0x7A
  0, // 0x7B
  1, // 0x7C
  0, // 0x7D
  1, // 0x7E
  0, // 0x7F
  0, // 0x80
  0, // 0x81
  0, // 0x82
  0, // 0x83
  0, // 0x84
  0, // 0x85
  0, // 0x86
  0, // 0x87
  0, // 0x88
  0, // 0x89
  0, // 0x8A
  0, // 0x8B
  0, // 0x8C
  0, // 0x8D
  0, // 0x8E
  0, // 0x8F
  0, // 0x90
  0, // 0x91
  0, // 0x92
  0, // 0x93
  0, // 0x94
  0, // 0x95
  0, // 0x96
  0, // 0x97
  0, // 0x98
  0, // 0x99
  0, // 0x9A
  0, // 0x9B
  0, // 0x9C
  0, // 0x9D
  0, // 0x9E
  0, // 0x9F
  0, // 0xA0
  0, // 0xA1
  0, // 0xA2
  0, // 0xA3
  0, // 0xA4
  0, // 0xA5
  0, // 0xA6
  0, // 0xA7
  0, // 0xA8
  0, // 0xA9
  0, // 0xAA
  0, // 0xAB
  0, // 0xAC
  0, // 0xAD
  0, // 0xAE
  0, // 0xAF
  0, // 0xB0
  0, // 0xB1
  0, // 0xB2
  0, // 0xB3
  0, // 0xB4
  0, // 0xB5
  0, // 0xB6
  0, // 0xB7
  0, // 0xB8
  0, // 0xB9
  0, // 0xBA
  0, // 0xBB
  0, // 0xBC
  0, // 0xBD
  0, // 0xBE
  0, // 0xBF
  0, // 0xC0
  0, // 0xC1
  0, // 0xC2
  0, // 0xC3
  0, // 0xC4
  0, // 0xC5
  0, // 0xC6
  0, // 0xC7
  0, // 0xC8
  0, // 0xC9
  0, // 0xCA
  0, // 0xCB
  0, // 0xCC
  0, // 0xCD
  0, // 0xCE
  0, // 0xCF
  0, // 0xD0
  0, // 0xD1
  0, // 0xD2
  0, // 0xD3
  0, // 0xD4
  0, // 0xD5
  0, // 0xD6
  0, // 0xD7
  0, // 0xD8
  0, // 0xD9
  0, // 0xDA
  0, // 0xDB
  0, // 0xDC
  0, // 0xDD
  0, // 0xDE
  0, // 0xDF
  0, // 0xE0
  0, // 0xE1
  0, // 0xE2
  0, // 0xE3
  0, // 0xE4
  0, // 0xE5
  0, // 0xE6
  0, // 0xE7
  0, // 0xE8
  0, // 0xE9
  0, // 0xEA
  0, // 0xEB
  0, // 0xEC
  0, // 0xED
  0, // 0xEE
  0, // 0xEF
  0, // 0xF0
  0, // 0xF1
  0, // 0xF2
  0, // 0xF3
  0, // 0xF4
  0, // 0xF5
  0, // 0xF6
  0, // 0xF7
  0, // 0xF8
  0, // 0xF9
  0, // 0xFA
  0, // 0xFB
  0, // 0xFC
  0, // 0xFD
  0, // 0xFE
  0, // 0xFF
}};

// ASCII punctuation classification.
static constexpr std::array<unsigned char, 256> kASCIIPunctuationCharacter = {{
  0, // 0x00
  0, // 0x01
  0, // 0x02
  0, // 0x03
  0, // 0x04
  0, // 0x05
  0, // 0x06
  0, // 0x07
  0, // 0x08
  0, // 0x09
  0, // 0x0A
  0, // 0x0B
  0, // 0x0C
  0, // 0x0D
  0, // 0x0E
  0, // 0x0F
  0, // 0x10
  0, // 0x11
  0, // 0x12
  0, // 0x13
  0, // 0x14
  0, // 0x15
  0, // 0x16
  0, // 0x17
  0, // 0x18
  0, // 0x19
  0, // 0x1A
  0, // 0x1B
  0, // 0x1C
  0, // 0x1D
  0, // 0x1E
  0, // 0x1F
  0, // 0x20
  1, // 0x21
  0, // 0x22
  1, // 0x23
  0, // 0x24
  1, // 0x25
  1, // 0x26
  0, // 0x27
  1, // 0x28
  1, // 0x29
  1, // 0x2A
  1, // 0x2B
  1, // 0x2C
  1, // 0x2D
  1, // 0x2E
  1, // 0x2F
  0, // 0x30
  0, // 0x31
  0, // 0x32
  0, // 0x33
  0, // 0x34
  0, // 0x35
  0, // 0x36
  0, // 0x37
  0, // 0x38
  0, // 0x39
  1, // 0x3A
  1, // 0x3B
  1, // 0x3C
  1, // 0x3D
  1, // 0x3E
  1, // 0x3F
  0, // 0x40
  0, // 0x41
  0, // 0x42
  0, // 0x43
  0, // 0x44
  0, // 0x45
  0, // 0x46
  0, // 0x47
  0, // 0x48
  0, // 0x49
  0, // 0x4A
  0, // 0x4B
  0, // 0x4C
  0, // 0x4D
  0, // 0x4E
  0, // 0x4F
  0, // 0x50
  0, // 0x51
  0, // 0x52
  0, // 0x53
  0, // 0x54
  0, // 0x55
  0, // 0x56
  0, // 0x57
  0, // 0x58
  0, // 0x59
  0, // 0x5A
  1, // 0x5B
  1, // 0x5C
  1, // 0x5D
  1, // 0x5E
  0, // 0x5F
  0, // 0x60
  0, // 0x61
  0, // 0x62
  0, // 0x63
  0, // 0x64
  0, // 0x65
  0, // 0x66
  0, // 0x67
  0, // 0x68
  0, // 0x69
  0, // 0x6A
  0, // 0x6B
  0, // 0x6C
  0, // 0x6D
  0, // 0x6E
  0, // 0x6F
  0, // 0x70
  0, // 0x71
  0, // 0x72
  0, // 0x73
  0, // 0x74
  0, // 0x75
  0, // 0x76
  0, // 0x77
  0, // 0x78
  0, // 0x79
  0, // 0x7A
  1, // 0x7B
  1, // 0x7C
  1, // 0x7D
  1, // 0x7E
  0, // 0x7F
  0, // 0x80
  0, // 0x81
  0, // 0x82
  0, // 0x83
  0, // 0x84
  0, // 0x85
  0, // 0x86
  0, // 0x87
  0, // 0x88
  0, // 0x89
  0, // 0x8A
  0, // 0x8B
  0, // 0x8C
  0, // 0x8D
  0, // 0x8E
  0, // 0x8F
  0, // 0x90
  0, // 0x91
  0, // 0x92
  0, // 0x93
  0, // 0x94
  0, // 0x95
  0, // 0x96
  0, // 0x97
  0, // 0x98
  0, // 0x99
  0, // 0x9A
  0, // 0x9B
  0, // 0x9C
  0, // 0x9D
  0, // 0x9E
  0, // 0x9F
  0, // 0xA0
  0, // 0xA1
  0, // 0xA2
  0, // 0xA3
  0, // 0xA4
  0, // 0xA5
  0, // 0xA6
  0, // 0xA7
  0, // 0xA8
  0, // 0xA9
  0, // 0xAA
  0, // 0xAB
  0, // 0xAC
  0, // 0xAD
  0, // 0xAE
  0, // 0xAF
  0, // 0xB0
  0, // 0xB1
  0, // 0xB2
  0, // 0xB3
  0, // 0xB4
  0, // 0xB5
  0, // 0xB6
  0, // 0xB7
  0, // 0xB8
  0, // 0xB9
  0, // 0xBA
  0, // 0xBB
  0, // 0xBC
  0, // 0xBD
  0, // 0xBE
  0, // 0xBF
  0, // 0xC0
  0, // 0xC1
  0, // 0xC2
  0, // 0xC3
  0, // 0xC4
  0, // 0xC5
  0, // 0xC6
  0, // 0xC7
  0, // 0xC8
  0, // 0xC9
  0, // 0xCA
  0, // 0xCB
  0, // 0xCC
  0, // 0xCD
  0, // 0xCE
  0, // 0xCF
  0, // 0xD0
  0, // 0xD1
  0, // 0xD2
  0, // 0xD3
  0, // 0xD4
  0, // 0xD5
  0, // 0xD6
  0, // 0xD7
  0, // 0xD8
  0, // 0xD9
  0, // 0xDA
  0, // 0xDB
  0, // 0xDC
  0, // 0xDD
  0, // 0xDE
  0, // 0xDF
  0, // 0xE0
  0, // 0xE1
  0, // 0xE2
  0, // 0xE3
  0, // 0xE4
  0, // 0xE5
  0, // 0xE6
  0, // 0xE7
  0, // 0xE8
  0, // 0xE9
  0, // 0xEA
  0, // 0xEB
  0, // 0xEC
  0, // 0xED
  0, // 0xEE
  0, // 0xEF
  0, // 0xF0
  0, // 0xF1
  0, // 0xF2
  0, // 0xF3
  0, // 0xF4
  0, // 0xF5
  0, // 0xF6
  0, // 0xF7
  0, // 0xF8
  0, // 0xF9
  0, // 0xFA
  0, // 0xFB
  0, // 0xFC
  0, // 0xFD
  0, // 0xFE
  0, // 0xFF
}};

// ASCII quote and escaped-identifier delimiters.
static constexpr std::array<unsigned char, 256> kASCIIQuoteCharacter = {{
  0, // 0x00
  0, // 0x01
  0, // 0x02
  0, // 0x03
  0, // 0x04
  0, // 0x05
  0, // 0x06
  0, // 0x07
  0, // 0x08
  0, // 0x09
  0, // 0x0A
  0, // 0x0B
  0, // 0x0C
  0, // 0x0D
  0, // 0x0E
  0, // 0x0F
  0, // 0x10
  0, // 0x11
  0, // 0x12
  0, // 0x13
  0, // 0x14
  0, // 0x15
  0, // 0x16
  0, // 0x17
  0, // 0x18
  0, // 0x19
  0, // 0x1A
  0, // 0x1B
  0, // 0x1C
  0, // 0x1D
  0, // 0x1E
  0, // 0x1F
  0, // 0x20
  0, // 0x21
  1, // 0x22
  0, // 0x23
  0, // 0x24
  0, // 0x25
  0, // 0x26
  1, // 0x27
  0, // 0x28
  0, // 0x29
  0, // 0x2A
  0, // 0x2B
  0, // 0x2C
  0, // 0x2D
  0, // 0x2E
  0, // 0x2F
  0, // 0x30
  0, // 0x31
  0, // 0x32
  0, // 0x33
  0, // 0x34
  0, // 0x35
  0, // 0x36
  0, // 0x37
  0, // 0x38
  0, // 0x39
  0, // 0x3A
  0, // 0x3B
  0, // 0x3C
  0, // 0x3D
  0, // 0x3E
  0, // 0x3F
  0, // 0x40
  0, // 0x41
  0, // 0x42
  0, // 0x43
  0, // 0x44
  0, // 0x45
  0, // 0x46
  0, // 0x47
  0, // 0x48
  0, // 0x49
  0, // 0x4A
  0, // 0x4B
  0, // 0x4C
  0, // 0x4D
  0, // 0x4E
  0, // 0x4F
  0, // 0x50
  0, // 0x51
  0, // 0x52
  0, // 0x53
  0, // 0x54
  0, // 0x55
  0, // 0x56
  0, // 0x57
  0, // 0x58
  0, // 0x59
  0, // 0x5A
  0, // 0x5B
  0, // 0x5C
  0, // 0x5D
  0, // 0x5E
  0, // 0x5F
  1, // 0x60
  0, // 0x61
  0, // 0x62
  0, // 0x63
  0, // 0x64
  0, // 0x65
  0, // 0x66
  0, // 0x67
  0, // 0x68
  0, // 0x69
  0, // 0x6A
  0, // 0x6B
  0, // 0x6C
  0, // 0x6D
  0, // 0x6E
  0, // 0x6F
  0, // 0x70
  0, // 0x71
  0, // 0x72
  0, // 0x73
  0, // 0x74
  0, // 0x75
  0, // 0x76
  0, // 0x77
  0, // 0x78
  0, // 0x79
  0, // 0x7A
  0, // 0x7B
  0, // 0x7C
  0, // 0x7D
  0, // 0x7E
  0, // 0x7F
  0, // 0x80
  0, // 0x81
  0, // 0x82
  0, // 0x83
  0, // 0x84
  0, // 0x85
  0, // 0x86
  0, // 0x87
  0, // 0x88
  0, // 0x89
  0, // 0x8A
  0, // 0x8B
  0, // 0x8C
  0, // 0x8D
  0, // 0x8E
  0, // 0x8F
  0, // 0x90
  0, // 0x91
  0, // 0x92
  0, // 0x93
  0, // 0x94
  0, // 0x95
  0, // 0x96
  0, // 0x97
  0, // 0x98
  0, // 0x99
  0, // 0x9A
  0, // 0x9B
  0, // 0x9C
  0, // 0x9D
  0, // 0x9E
  0, // 0x9F
  0, // 0xA0
  0, // 0xA1
  0, // 0xA2
  0, // 0xA3
  0, // 0xA4
  0, // 0xA5
  0, // 0xA6
  0, // 0xA7
  0, // 0xA8
  0, // 0xA9
  0, // 0xAA
  0, // 0xAB
  0, // 0xAC
  0, // 0xAD
  0, // 0xAE
  0, // 0xAF
  0, // 0xB0
  0, // 0xB1
  0, // 0xB2
  0, // 0xB3
  0, // 0xB4
  0, // 0xB5
  0, // 0xB6
  0, // 0xB7
  0, // 0xB8
  0, // 0xB9
  0, // 0xBA
  0, // 0xBB
  0, // 0xBC
  0, // 0xBD
  0, // 0xBE
  0, // 0xBF
  0, // 0xC0
  0, // 0xC1
  0, // 0xC2
  0, // 0xC3
  0, // 0xC4
  0, // 0xC5
  0, // 0xC6
  0, // 0xC7
  0, // 0xC8
  0, // 0xC9
  0, // 0xCA
  0, // 0xCB
  0, // 0xCC
  0, // 0xCD
  0, // 0xCE
  0, // 0xCF
  0, // 0xD0
  0, // 0xD1
  0, // 0xD2
  0, // 0xD3
  0, // 0xD4
  0, // 0xD5
  0, // 0xD6
  0, // 0xD7
  0, // 0xD8
  0, // 0xD9
  0, // 0xDA
  0, // 0xDB
  0, // 0xDC
  0, // 0xDD
  0, // 0xDE
  0, // 0xDF
  0, // 0xE0
  0, // 0xE1
  0, // 0xE2
  0, // 0xE3
  0, // 0xE4
  0, // 0xE5
  0, // 0xE6
  0, // 0xE7
  0, // 0xE8
  0, // 0xE9
  0, // 0xEA
  0, // 0xEB
  0, // 0xEC
  0, // 0xED
  0, // 0xEE
  0, // 0xEF
  0, // 0xF0
  0, // 0xF1
  0, // 0xF2
  0, // 0xF3
  0, // 0xF4
  0, // 0xF5
  0, // 0xF6
  0, // 0xF7
  0, // 0xF8
  0, // 0xF9
  0, // 0xFA
  0, // 0xFB
  0, // 0xFC
  0, // 0xFD
  0, // 0xFE
  0, // 0xFF
}};

// ASCII line-break classification.
static constexpr std::array<unsigned char, 256> kASCIILineBreak = {{
  0, // 0x00
  0, // 0x01
  0, // 0x02
  0, // 0x03
  0, // 0x04
  0, // 0x05
  0, // 0x06
  0, // 0x07
  0, // 0x08
  0, // 0x09
  1, // 0x0A
  0, // 0x0B
  0, // 0x0C
  1, // 0x0D
  0, // 0x0E
  0, // 0x0F
  0, // 0x10
  0, // 0x11
  0, // 0x12
  0, // 0x13
  0, // 0x14
  0, // 0x15
  0, // 0x16
  0, // 0x17
  0, // 0x18
  0, // 0x19
  0, // 0x1A
  0, // 0x1B
  0, // 0x1C
  0, // 0x1D
  0, // 0x1E
  0, // 0x1F
  0, // 0x20
  0, // 0x21
  0, // 0x22
  0, // 0x23
  0, // 0x24
  0, // 0x25
  0, // 0x26
  0, // 0x27
  0, // 0x28
  0, // 0x29
  0, // 0x2A
  0, // 0x2B
  0, // 0x2C
  0, // 0x2D
  0, // 0x2E
  0, // 0x2F
  0, // 0x30
  0, // 0x31
  0, // 0x32
  0, // 0x33
  0, // 0x34
  0, // 0x35
  0, // 0x36
  0, // 0x37
  0, // 0x38
  0, // 0x39
  0, // 0x3A
  0, // 0x3B
  0, // 0x3C
  0, // 0x3D
  0, // 0x3E
  0, // 0x3F
  0, // 0x40
  0, // 0x41
  0, // 0x42
  0, // 0x43
  0, // 0x44
  0, // 0x45
  0, // 0x46
  0, // 0x47
  0, // 0x48
  0, // 0x49
  0, // 0x4A
  0, // 0x4B
  0, // 0x4C
  0, // 0x4D
  0, // 0x4E
  0, // 0x4F
  0, // 0x50
  0, // 0x51
  0, // 0x52
  0, // 0x53
  0, // 0x54
  0, // 0x55
  0, // 0x56
  0, // 0x57
  0, // 0x58
  0, // 0x59
  0, // 0x5A
  0, // 0x5B
  0, // 0x5C
  0, // 0x5D
  0, // 0x5E
  0, // 0x5F
  0, // 0x60
  0, // 0x61
  0, // 0x62
  0, // 0x63
  0, // 0x64
  0, // 0x65
  0, // 0x66
  0, // 0x67
  0, // 0x68
  0, // 0x69
  0, // 0x6A
  0, // 0x6B
  0, // 0x6C
  0, // 0x6D
  0, // 0x6E
  0, // 0x6F
  0, // 0x70
  0, // 0x71
  0, // 0x72
  0, // 0x73
  0, // 0x74
  0, // 0x75
  0, // 0x76
  0, // 0x77
  0, // 0x78
  0, // 0x79
  0, // 0x7A
  0, // 0x7B
  0, // 0x7C
  0, // 0x7D
  0, // 0x7E
  0, // 0x7F
  0, // 0x80
  0, // 0x81
  0, // 0x82
  0, // 0x83
  0, // 0x84
  0, // 0x85
  0, // 0x86
  0, // 0x87
  0, // 0x88
  0, // 0x89
  0, // 0x8A
  0, // 0x8B
  0, // 0x8C
  0, // 0x8D
  0, // 0x8E
  0, // 0x8F
  0, // 0x90
  0, // 0x91
  0, // 0x92
  0, // 0x93
  0, // 0x94
  0, // 0x95
  0, // 0x96
  0, // 0x97
  0, // 0x98
  0, // 0x99
  0, // 0x9A
  0, // 0x9B
  0, // 0x9C
  0, // 0x9D
  0, // 0x9E
  0, // 0x9F
  0, // 0xA0
  0, // 0xA1
  0, // 0xA2
  0, // 0xA3
  0, // 0xA4
  0, // 0xA5
  0, // 0xA6
  0, // 0xA7
  0, // 0xA8
  0, // 0xA9
  0, // 0xAA
  0, // 0xAB
  0, // 0xAC
  0, // 0xAD
  0, // 0xAE
  0, // 0xAF
  0, // 0xB0
  0, // 0xB1
  0, // 0xB2
  0, // 0xB3
  0, // 0xB4
  0, // 0xB5
  0, // 0xB6
  0, // 0xB7
  0, // 0xB8
  0, // 0xB9
  0, // 0xBA
  0, // 0xBB
  0, // 0xBC
  0, // 0xBD
  0, // 0xBE
  0, // 0xBF
  0, // 0xC0
  0, // 0xC1
  0, // 0xC2
  0, // 0xC3
  0, // 0xC4
  0, // 0xC5
  0, // 0xC6
  0, // 0xC7
  0, // 0xC8
  0, // 0xC9
  0, // 0xCA
  0, // 0xCB
  0, // 0xCC
  0, // 0xCD
  0, // 0xCE
  0, // 0xCF
  0, // 0xD0
  0, // 0xD1
  0, // 0xD2
  0, // 0xD3
  0, // 0xD4
  0, // 0xD5
  0, // 0xD6
  0, // 0xD7
  0, // 0xD8
  0, // 0xD9
  0, // 0xDA
  0, // 0xDB
  0, // 0xDC
  0, // 0xDD
  0, // 0xDE
  0, // 0xDF
  0, // 0xE0
  0, // 0xE1
  0, // 0xE2
  0, // 0xE3
  0, // 0xE4
  0, // 0xE5
  0, // 0xE6
  0, // 0xE7
  0, // 0xE8
  0, // 0xE9
  0, // 0xEA
  0, // 0xEB
  0, // 0xEC
  0, // 0xED
  0, // 0xEE
  0, // 0xEF
  0, // 0xF0
  0, // 0xF1
  0, // 0xF2
  0, // 0xF3
  0, // 0xF4
  0, // 0xF5
  0, // 0xF6
  0, // 0xF7
  0, // 0xF8
  0, // 0xF9
  0, // 0xFA
  0, // 0xFB
  0, // 0xFC
  0, // 0xFD
  0, // 0xFE
  0, // 0xFF
}};

static constexpr bool isASCIIWhitespaceByte(unsigned char byte) noexcept {
  return kASCIIWhitespace[byte] != 0;
}

static constexpr bool isASCIILineBreakByte(unsigned char byte) noexcept {
  return kASCIILineBreak[byte] != 0;
}

static constexpr bool isASCIIIdentifierStartByte(unsigned char byte) noexcept {
  return kASCIIIdentifierStart[byte] != 0;
}

static constexpr bool isASCIIIdentifierContinueByte(unsigned char byte) noexcept {
  return kASCIIIdentifierContinue[byte] != 0;
}

static constexpr unsigned asciiDigitValue(unsigned char byte) noexcept {
  return kASCIIDigitValue[byte];
}

static constexpr bool isASCIIOperatorByte(unsigned char byte) noexcept {
  return kASCIIOperatorCharacter[byte] != 0;
}

static constexpr bool isASCIIPunctuationByte(unsigned char byte) noexcept {
  return kASCIIPunctuationCharacter[byte] != 0;
}

static constexpr bool isASCIIQuoteByte(unsigned char byte) noexcept {
  return kASCIIQuoteCharacter[byte] != 0;
}

static constexpr bool isASCIIControlByte(unsigned char byte) noexcept {
  return byte < 0x20u && !isASCIIWhitespaceByte(byte);
}

static constexpr bool isASCIIHexDigitByte(unsigned char byte) noexcept {
  return asciiDigitValue(byte) < 16u;
}

static constexpr bool isASCIIDecimalDigitByte(unsigned char byte) noexcept {
  return asciiDigitValue(byte) < 10u;
}

static constexpr bool isASCIIAlphaByte(unsigned char byte) noexcept {
  return (byte >= static_cast<unsigned char>('A') &&
          byte <= static_cast<unsigned char>('Z')) ||
         (byte >= static_cast<unsigned char>('a') &&
          byte <= static_cast<unsigned char>('z'));
}

static constexpr bool isASCIIAlphaNumericByte(unsigned char byte) noexcept {
  return isASCIIAlphaByte(byte) || isASCIIDecimalDigitByte(byte);
}

static constexpr bool isASCIIUnderscoreByte(unsigned char byte) noexcept {
  return byte == static_cast<unsigned char>('_');
}

static constexpr bool isASCIIAtByte(unsigned char byte) noexcept {
  return byte == static_cast<unsigned char>('@');
}

static constexpr bool isASCIIHashByte(unsigned char byte) noexcept {
  return byte == static_cast<unsigned char>('#');
}

static constexpr bool isASCIIBackslashByte(unsigned char byte) noexcept {
  return byte == static_cast<unsigned char>('\\');
}

static constexpr bool isASCIIQuoteOrBacktickByte(unsigned char byte) noexcept {
  return isASCIIQuoteByte(byte);
}

static constexpr bool isASCIICommentLeadByte(unsigned char byte) noexcept {
  return byte == static_cast<unsigned char>('/');
}

static constexpr bool isASCIINumericLeadByte(unsigned char byte) noexcept {
  return isASCIIDecimalDigitByte(byte);
}

static constexpr bool isASCIIIdentifierLeadByte(unsigned char byte) noexcept {
  return isASCIIIdentifierStartByte(byte);
}

} // namespace

Lexer::Lexer(std::string_view source, LexerOptions options)
    : source_(source), options_(options) {
  // HyperLang source is UTF-8. A BOM is accepted only at the beginning of the
  // buffer and is treated as source metadata rather than a token.
  consumeUTF8BOM();
}

SourceLocation Lexer::currentLocation() const {
  return {cursor_, line_, column_};
}

char Lexer::currentByte() const {
  return cursor_ < source_.size() ? source_[cursor_] : '\0';
}

char Lexer::peekByte(std::size_t distance) const {
  const std::size_t index = cursor_ + distance;
  return index < source_.size() ? source_[index] : '\0';
}

void Lexer::advanceByte() {
  if (cursor_ >= source_.size())
    return;

  const unsigned char byte =
      static_cast<unsigned char>(source_[cursor_++]);

  if (byte == '\r') {
    ++line_;
    column_ = 1;
    return;
  }

  if (byte == '\n') {
    if (cursor_ < 2 || source_[cursor_ - 2] != '\r') {
      ++line_;
      column_ = 1;
    }
    return;
  }

  ++column_;
}

void Lexer::advanceBytes(std::size_t count) {
  const std::size_t remaining = source_.size() - cursor_;
  count = std::min(count, remaining);

  while (count != 0) {
    advanceByte();
    --count;
  }
}

bool Lexer::consumeIf(char ch) {
  if (currentByte() != ch)
    return false;
  advanceByte();
  return true;
}

void Lexer::emit(Diagnostic::Severity severity, SourceLocation location,
                 std::string message) {
  diagnostics_.push_back({severity, location, std::move(message)});
}

void Lexer::error(SourceLocation location, std::string message) {
  emit(Diagnostic::Severity::Error, location, std::move(message));
}

void Lexer::warning(SourceLocation location, std::string message) {
  emit(Diagnostic::Severity::Warning, location, std::move(message));
}

Token Lexer::makeToken(tok::Kind kind, std::size_t start,
                       SourceLocation startLocation) const {
  Token token;
  token.kind = kind;
  token.range.start = startLocation;
  token.range.end = currentLocation();
  token.text = source_.substr(start, cursor_ - start);
  return token;
}

Token Lexer::lex() {
  if (lookahead_) {
    Token token = std::move(*lookahead_);
    lookahead_.reset();
    return token;
  }
  return lexImpl();
}

const Token &Lexer::peek() {
  if (!lookahead_)
    lookahead_ = lexImpl();
  return *lookahead_;
}

Token Lexer::lexIf(tok::Kind kind) {
  if (peek().kind != kind)
    return {};
  return lex();
}

bool Lexer::atEnd() const {
  return cursor_ >= source_.size();
}

std::size_t Lexer::offset() const {
  return cursor_;
}

SourceLocation Lexer::location() const {
  return currentLocation();
}

const std::vector<Diagnostic> &Lexer::diagnostics() const {
  return diagnostics_;
}

bool Lexer::hasErrors() const {
  for (const Diagnostic &diagnostic : diagnostics_)
    if (diagnostic.severity == Diagnostic::Severity::Error)
      return true;
  return false;
}

void Lexer::reset(std::size_t offset) {
  cursor_ = std::min(offset, source_.size());
  line_ = 1;
  column_ = 1;
  lookahead_.reset();
  diagnostics_.clear();

  for (std::size_t index = 0; index < cursor_; ++index) {
    if (source_[index] == '\r') {
      ++line_;
      column_ = 1;
      continue;
    }
    if (source_[index] == '\n') {
      if (index == 0 || source_[index - 1] != '\r') {
        ++line_;
        column_ = 1;
      }
      continue;
    }
    ++column_;
  }
}

void Lexer::setRetainComments(bool enabled) {
  options_.retainComments = enabled;
}

tok::Kind Lexer::classifyIdentifier(std::string_view spelling) {
  return lexer::classifyKeyword(spelling);
}

bool Lexer::isKeyword(std::string_view identifier) {
  return lexer::isKeywordSpelling(identifier);
}

bool Lexer::isAttribute(std::string_view spelling) {
  return lexer::isAttributeSpelling(spelling);
}

bool Lexer::isOperatorCharacter(char ch) {
  return lexer::isOperatorCharacter(ch);
}

Token Lexer::lexIdentifierOrKeyword() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  scanIdentifier();

  const std::string_view spelling = source_.substr(start, cursor_ - start);
  Token token = makeToken(classifyIdentifier(spelling), start, loc);
  token.decodedText = decodeIdentifier(spelling);
  return token;
}

Token Lexer::lexEscapedIdentifier() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();

  advanceByte();
  const std::size_t valueStart = cursor_;

  while (!atEnd() && static_cast<unsigned char>(currentByte()) != 96) {
    const unicode::DecodeResult result = unicode::decode(source_, cursor_);
    if (!result.valid) {
      error(currentLocation(), "invalid UTF-8 sequence in escaped identifier");
      advanceByte();
      continue;
    }
    advanceBytes(result.width);
  }

  Token token = makeToken(tok::Kind::EscapedIdentifier, start, loc);
  token.decodedText = std::string(source_.substr(valueStart, cursor_ - valueStart));

  if (!consumeIf(static_cast<char>(96)))
    error(loc, "unterminated escaped identifier");

  token.range.end = currentLocation();
  token.text = source_.substr(start, cursor_ - start);
  return token;
}

Token Lexer::lexAttributedName() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();

  advanceByte();
  const std::size_t nameStart = cursor_;
  while (isAsciiIdentifierContinue(currentByte()))
    advanceByte();

  if (cursor_ == nameStart)
    return makeToken(tok::Kind::At, start, loc);

  const std::string_view spelling = source_.substr(start, cursor_ - start);
  for (const lexer::AttributeEntry &entry : lexer::AttributeTable)
    if (entry.spelling == spelling)
      return makeToken(entry.kind, start, loc);

  Token token = makeToken(tok::Kind::AttributedName, start, loc);
  token.decodedText = std::string(spelling.substr(1));
  return token;
}

void Lexer::finalizeInteger(Token &token, unsigned base) {
  std::string normalized;
  normalized.reserve(token.text.size());

  for (char ch : token.text)
    if (ch != '_')
      normalized.push_back(ch);

  std::string_view digits = normalized;
  if (base != 10 && digits.size() >= 2)
    digits.remove_prefix(2);

  std::uint64_t value = 0;
  const auto parsed = std::from_chars(
      digits.data(), digits.data() + digits.size(), value, base);

  if (parsed.ec == std::errc{}) {
    token.integerValue = value;
    token.hasIntegerValue = true;
  } else {
    error(token.range.start, "integer literal is outside the supported range");
  }
}

void Lexer::finalizeFloat(Token &token) {
  std::string normalized;
  normalized.reserve(token.text.size());

  for (char ch : token.text)
    if (ch != '_')
      normalized.push_back(ch);

  if (!normalized.empty() &&
      (normalized.back() == 'f' || normalized.back() == 'F'))
    normalized.pop_back();

  const bool hexadecimal =
      normalized.size() >= 2 &&
      normalized[0] == '0' &&
      (normalized[1] == 'x' || normalized[1] == 'X');

  const char *first = normalized.data();
  const char *last = normalized.data() + normalized.size();
  if (hexadecimal) {
    first += 2;
    if (first == last) {
      error(token.range.start, "invalid hexadecimal floating-point literal");
      return;
    }
  }

  double value = 0.0;
  const auto parsed = std::from_chars(
      first, last, value,
      hexadecimal ? std::chars_format::hex : std::chars_format::general);

  if (parsed.ec == std::errc{} && parsed.ptr == last) {
    token.floatingValue = value;
    token.hasFloatingValue = true;
    return;
  }

  if (parsed.ec == std::errc::result_out_of_range)
    error(token.range.start, "floating-point literal is outside the supported range");
  else
    error(token.range.start, "invalid floating-point literal");
}

Token Lexer::lexNumber() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  unsigned base = 10;
  bool floating = false;
  bool hexadecimal = false;

  if (currentByte() == '0') {
    const char prefix = peekByte();
    if (prefix == 'x' || prefix == 'X') {
      base = 16;
      hexadecimal = true;
      advanceBytes(2);
      const std::size_t digitsStart = cursor_;
      scanDigits(16, true);

      if (currentByte() == '.') {
        floating = true;
        advanceByte();
        scanDigits(16, false);
      }

      if (currentByte() == 'p' || currentByte() == 'P') {
        floating = true;
        scanExponent();
      } else if (floating) {
        error(loc, "hexadecimal floating-point literal requires a binary exponent");
      }

      if (cursor_ == digitsStart && !floating)
        error(loc, "expected hexadecimal digits after '0x'");
    } else if (prefix == 'b' || prefix == 'B') {
      base = 2;
      advanceBytes(2);
      const std::size_t digitsStart = cursor_;
      scanDigits(2, true);
      if (cursor_ == digitsStart)
        error(loc, "expected binary digits after '0b'");
    } else if (prefix == 'o' || prefix == 'O') {
      base = 8;
      advanceBytes(2);
      const std::size_t digitsStart = cursor_;
      scanDigits(8, true);
      if (cursor_ == digitsStart)
        error(loc, "expected octal digits after '0o'");
    } else {
      scanDigits(10, true);
      if (currentByte() == '.' && peekByte() != '.') {
        floating = true;
        advanceByte();
        scanDigits(10, false);
      }
      if (currentByte() == 'e' || currentByte() == 'E') {
        floating = true;
        scanExponent();
      }
    }
  } else {
    scanDigits(10, true);
    if (currentByte() == '.' && peekByte() != '.') {
      floating = true;
      advanceByte();
      scanDigits(10, false);
    }
    if (currentByte() == 'e' || currentByte() == 'E') {
      floating = true;
      scanExponent();
    }
  }

  const bool hasFloatSuffix = (floating || !hexadecimal) && scanFloatSuffix();
  floating = floating || hasFloatSuffix;

  const std::string_view literal = source_.substr(start, cursor_ - start);
  if (lexer::hasInvalidNumericSeparators(literal))
    error(loc, "invalid placement of numeric separator");

  if (!literal.empty() &&
      (isAsciiIdentifierContinue(currentByte()) ||
       (static_cast<unsigned char>(currentByte()) >= 0x80u &&
        lexer::isUnicodeIdentifierContinue(source_, cursor_)))) {
    error(currentLocation(), "invalid character after numeric literal");
  }

  Token token = makeToken(
      floating ? tok::Kind::FloatingLiteral : tok::Kind::IntegerLiteral,
      start, loc);

  if (floating)
    finalizeFloat(token);
  else
    finalizeInteger(token, base);

  (void)hexadecimal;
  return token;
}

bool Lexer::decodeHexEscape(std::string &output, unsigned digits) {
  std::uint32_t value = 0;

  for (unsigned index = 0; index < digits; ++index) {
    const unsigned digit = digitValue(currentByte());
    if (digit >= 16) {
      error(currentLocation(), "invalid hexadecimal escape");
      return false;
    }
    value = (value << 4) | digit;
    advanceByte();
  }

  if (!unicode::isValidScalar(value)) {
    error(currentLocation(), "escape does not name a valid Unicode scalar");
    return false;
  }

  char bytes[4]{};
  const std::size_t length = unicode::encodedLength(value);

  if (length == 1) {
    bytes[0] = static_cast<char>(value);
  } else if (length == 2) {
    bytes[0] = static_cast<char>(0xC0 | (value >> 6));
    bytes[1] = static_cast<char>(0x80 | (value & 0x3F));
  } else if (length == 3) {
    bytes[0] = static_cast<char>(0xE0 | (value >> 12));
    bytes[1] = static_cast<char>(0x80 | ((value >> 6) & 0x3F));
    bytes[2] = static_cast<char>(0x80 | (value & 0x3F));
  } else {
    bytes[0] = static_cast<char>(0xF0 | (value >> 18));
    bytes[1] = static_cast<char>(0x80 | ((value >> 12) & 0x3F));
    bytes[2] = static_cast<char>(0x80 | ((value >> 6) & 0x3F));
    bytes[3] = static_cast<char>(0x80 | (value & 0x3F));
  }

  output.append(bytes, length);
  return true;
}

bool Lexer::decodeUnicodeEscape(std::string &output) {
  if (!consumeIf('{')) {
    error(currentLocation(), "expected an opening brace in Unicode escape");
    return false;
  }

  std::uint32_t value = 0;
  unsigned digits = 0;

  while (!atEnd() && currentByte() != '}') {
    const unsigned digit = digitValue(currentByte());
    if (digit >= 16 || ++digits > 6) {
      error(currentLocation(), "invalid Unicode escape");
      return false;
    }
    value = (value << 4) | digit;
    advanceByte();
  }

  if (!consumeIf('}')) {
    error(currentLocation(), "unterminated Unicode escape");
    return false;
  }

  if (digits == 0 || !unicode::isValidScalar(value)) {
    error(currentLocation(), "invalid Unicode scalar value");
    return false;
  }

  const std::size_t length = unicode::encodedLength(value);
  char bytes[4]{};

  if (length == 1) {
    bytes[0] = static_cast<char>(value);
  } else if (length == 2) {
    bytes[0] = static_cast<char>(0xC0 | (value >> 6));
    bytes[1] = static_cast<char>(0x80 | (value & 0x3F));
  } else if (length == 3) {
    bytes[0] = static_cast<char>(0xE0 | (value >> 12));
    bytes[1] = static_cast<char>(0x80 | ((value >> 6) & 0x3F));
    bytes[2] = static_cast<char>(0x80 | (value & 0x3F));
  } else {
    bytes[0] = static_cast<char>(0xF0 | (value >> 18));
    bytes[1] = static_cast<char>(0x80 | ((value >> 12) & 0x3F));
    bytes[2] = static_cast<char>(0x80 | ((value >> 6) & 0x3F));
    bytes[3] = static_cast<char>(0x80 | (value & 0x3F));
  }

  output.append(bytes, length);
  return true;
}

bool Lexer::decodeQuotedEscape(std::string &output) {
  if (atEnd())
    return false;

  const char escaped = currentByte();
  advanceByte();

  switch (escaped) {
  case 'n': output.push_back('\n'); return true;
  case 'r': output.push_back('\r'); return true;
  case 't': output.push_back('\t'); return true;
  case 'b': output.push_back('\b'); return true;
  case 'f': output.push_back('\f'); return true;
  case '0': output.push_back('\0'); return true;
  case '\\': output.push_back('\\'); return true;
  case '"': output.push_back('"'); return true;
  case 39: output.push_back(39); return true;
  case 'u': return decodeUnicodeEscape(output);
  case 'x': return decodeHexEscape(output, 2);
  default:
    error(currentLocation(), "unknown escape sequence");
    output.push_back(escaped);
    return false;
  }
}

bool Lexer::decodeEscape(std::string &output) {
  if (!consumeIf('\\'))
    return false;
  return decodeQuotedEscape(output);
}

Token Lexer::lexString() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();

  std::string value;

  while (!atEnd()) {
    if (currentByte() == '"') {
      advanceByte();
      Token token = makeToken(tok::Kind::StringLiteral, start, loc);
      token.decodedText = std::move(value);
      return token;
    }

    if (currentByte() == '\\') {
      decodeEscape(value);
      continue;
    }

    if (currentByte() == '\n') {
      error(currentLocation(), "newline is not permitted in a string literal");
      break;
    }

    const unicode::DecodeResult character = unicode::decode(source_, cursor_);
    if (!character.valid) {
      error(currentLocation(), "invalid UTF-8 sequence in string literal");
      advanceByte();
      continue;
    }

    value.append(source_.substr(cursor_, character.width));
    advanceBytes(character.width);
  }

  error(loc, "unterminated string literal");
  Token token = makeToken(tok::Kind::StringLiteral, start, loc);
  token.decodedText = std::move(value);
  return token;
}

Token Lexer::lexCharacter() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();

  std::string value;

  if (currentByte() == '\\') {
    decodeEscape(value);
  } else if (!atEnd()) {
    const unicode::DecodeResult character = unicode::decode(source_, cursor_);
    if (character.valid) {
      value.assign(source_.substr(cursor_, character.width));
      advanceBytes(character.width);
    } else {
      error(currentLocation(), "invalid UTF-8 sequence in character literal");
      advanceByte();
    }
  }

  if (!consumeIf(static_cast<char>(39)))
    error(loc, "unterminated character literal");

  if (!lexer::isSingleUnicodeScalar(value))
    error(loc, "character literal must contain exactly one Unicode scalar");

  Token token = makeToken(tok::Kind::CharacterLiteral, start, loc);
  token.decodedText = std::move(value);
  return token;
}

bool Lexer::skipLineComment() {
  if (currentByte() == '/' && peekByte() == '/') {
    advanceBytes(2);
    while (!atEnd() && currentByte() != '\n' && currentByte() != '\r')
      advanceByte();
    return true;
  }

  return false;
}

bool Lexer::skipBlockComment() {
  if (currentByte() != '/' || peekByte() != '*')
    return false;

  const SourceLocation loc = currentLocation();
  advanceBytes(2);
  unsigned depth = 1;

  while (!atEnd()) {
    if (options_.allowNestedBlockComments &&
        currentByte() == '/' && peekByte() == '*') {
      ++depth;
      advanceBytes(2);
      continue;
    }

    if (currentByte() == '*' && peekByte() == '/') {
      advanceBytes(2);
      if (--depth == 0)
        return true;
      continue;
    }

    advanceByte();
  }

  error(loc, "unterminated block comment");
  return true;
}

bool Lexer::skipHashbang() {
  if (!options_.allowHashbang || cursor_ != 0 ||
      currentByte() != '#' || peekByte() != '!')
    return false;

  while (!atEnd() && currentByte() != '\n')
    advanceByte();

  return true;
}

bool Lexer::skipTrivia() {
  bool consumed = false;

  for (;;) {
    while (!atEnd()) {
      const unsigned char byte = static_cast<unsigned char>(currentByte());
      if (byte >= 0x80u)
        break;
      if (!isASCIIWhitespaceByte(byte))
        break;
      advanceByte();
      consumed = true;
    }

    if (!atEnd() && static_cast<unsigned char>(currentByte()) >= 0x80u) {
      const unicode::DecodeResult character = unicode::decode(source_, cursor_);
      if (character.valid && unicode::isWhitespace(character.codePoint)) {
        advanceBytes(character.width);
        consumed = true;
        continue;
      }
    }

    if (skipHashbang()) {
      consumed = true;
      continue;
    }
    if (options_.retainComments && lexer::isCommentStart(source_, cursor_))
      break;
    if (skipLineComment()) {
      consumed = true;
      continue;
    }
    if (skipBlockComment()) {
      consumed = true;
      continue;
    }
    break;
  }
  return consumed;
}

bool Lexer::isAtLineStart() const {
  return column_ == 1;
}

bool Lexer::consumeUTF8BOM() {
  if (cursor_ != 0 || source_.size() < 3)
    return false;
  if (static_cast<unsigned char>(source_[0]) != 0xEF ||
      static_cast<unsigned char>(source_[1]) != 0xBB ||
      static_cast<unsigned char>(source_[2]) != 0xBF)
    return false;
  cursor_ = 3;
  line_ = 1;
  column_ = 1;
  return true;
}

bool Lexer::scanUTF8CodePoint() {
  if (atEnd())
    return false;
  const unicode::DecodeResult result = unicode::decode(source_, cursor_);
  if (!result.valid) {
    const SourceLocation loc = currentLocation();
    const unsigned char byte = static_cast<unsigned char>(currentByte());
    error(loc, "invalid UTF-8 byte " + hexByte(byte));
    advanceByte();
    return false;
  }
  advanceBytes(result.width);
  return true;
}

bool Lexer::skipHorizontalWhitespace() {
  bool consumed = false;
  for (;;) {
    if (atEnd())
      break;
    const unsigned char byte = static_cast<unsigned char>(currentByte());
    if (byte < 0x80u) {
      if (isASCIIWhitespaceByte(byte) && !isASCIILineBreakByte(byte)) {
        advanceByte();
        consumed = true;
        continue;
      }
      break;
    }
    if (!lexer::isUnicodeWhitespace(source_, cursor_))
      break;
    const unicode::DecodeResult result = unicode::decode(source_, cursor_);
    advanceBytes(result.width);
    consumed = true;
  }
  return consumed;
}

Token Lexer::makeEOFToken() const {
  Token token;
  token.kind = tok::Kind::EndOfFile;
  token.range.start = currentLocation();
  token.range.end = currentLocation();
  return token;
}

Token Lexer::lexComment() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();

  if (currentByte() == '/' && peekByte() == '/') {
    advanceBytes(2);
    while (!atEnd() && currentByte() != '\n' && currentByte() != '\r')
      advanceByte();
    Token token = makeToken(tok::Kind::Comment, start, loc);
    token.decodedText = std::string(source_.substr(start + 2,
                                             cursor_ - start - 2));
    return token;
  }

  advanceBytes(2);
  unsigned depth = 1;
  while (!atEnd()) {
    if (options_.allowNestedBlockComments &&
        currentByte() == '/' && peekByte() == '*') {
      ++depth;
      advanceBytes(2);
      continue;
    }
    if (currentByte() == '*' && peekByte() == '/') {
      advanceBytes(2);
      if (--depth == 0)
        break;
      continue;
    }
    if (!scanUTF8CodePoint())
      continue;
  }

  if (depth != 0)
    error(loc, "unterminated block comment");

  const std::size_t contentStart = start + 2;
  const std::size_t contentEnd =
      cursor_ >= 2 && source_[cursor_ - 2] == '*' &&
      source_[cursor_ - 1] == '/' ? cursor_ - 2 : cursor_;
  Token token = makeToken(tok::Kind::Comment, start, loc);
  token.decodedText = std::string(
      source_.substr(contentStart, contentEnd - contentStart));
  return token;
}
Token Lexer::lexSlash() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('=')) return makeToken(tok::Kind::SlashEqual, start, loc);
  return makeToken(tok::Kind::Slash, start, loc);
}

Token Lexer::lexDot() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('.')) {
    if (consumeIf('<')) return makeToken(tok::Kind::DotDotLess, start, loc);
    return makeToken(tok::Kind::DotDot, start, loc);
  }
  return makeToken(tok::Kind::Dot, start, loc);
}

Token Lexer::lexEqual() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('=')) return makeToken(tok::Kind::EqualEqual, start, loc);
  if (consumeIf('>')) return makeToken(tok::Kind::FatArrow, start, loc);
  return makeToken(tok::Kind::Equal, start, loc);
}

Token Lexer::lexBang() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('=')) return makeToken(tok::Kind::NotEqual, start, loc);
  return makeToken(tok::Kind::Bang, start, loc);
}

Token Lexer::lexLess() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('=')) return makeToken(tok::Kind::LessEqual, start, loc);
  if (consumeIf('<')) {
    if (consumeIf('=')) return makeToken(tok::Kind::ShiftLeftEqual, start, loc);
    return makeToken(tok::Kind::ShiftLeft, start, loc);
  }
  return makeToken(tok::Kind::Less, start, loc);
}

Token Lexer::lexGreater() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('=')) return makeToken(tok::Kind::GreaterEqual, start, loc);
  if (consumeIf('>')) {
    if (consumeIf('=')) return makeToken(tok::Kind::ShiftRightEqual, start, loc);
    return makeToken(tok::Kind::ShiftRight, start, loc);
  }
  return makeToken(tok::Kind::Greater, start, loc);
}

Token Lexer::lexPlus() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('+')) return makeToken(tok::Kind::Increment, start, loc);
  if (consumeIf('=')) return makeToken(tok::Kind::PlusEqual, start, loc);
  return makeToken(tok::Kind::Plus, start, loc);
}

Token Lexer::lexMinus() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('-')) return makeToken(tok::Kind::Decrement, start, loc);
  if (consumeIf('=')) return makeToken(tok::Kind::MinusEqual, start, loc);
  if (consumeIf('>')) return makeToken(tok::Kind::Arrow, start, loc);
  return makeToken(tok::Kind::Minus, start, loc);
}

Token Lexer::lexStar() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('=')) return makeToken(tok::Kind::StarEqual, start, loc);
  return makeToken(tok::Kind::Star, start, loc);
}

Token Lexer::lexPercent() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('=')) return makeToken(tok::Kind::PercentEqual, start, loc);
  return makeToken(tok::Kind::Percent, start, loc);
}

Token Lexer::lexAmpersand() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('&')) return makeToken(tok::Kind::AmpersandAmpersand, start, loc);
  if (consumeIf('=')) return makeToken(tok::Kind::AmpersandEqual, start, loc);
  return makeToken(tok::Kind::Ampersand, start, loc);
}

Token Lexer::lexPipe() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('|')) return makeToken(tok::Kind::PipePipe, start, loc);
  if (consumeIf('=')) return makeToken(tok::Kind::PipeEqual, start, loc);
  return makeToken(tok::Kind::Pipe, start, loc);
}

Token Lexer::lexCaret() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('=')) return makeToken(tok::Kind::CaretEqual, start, loc);
  return makeToken(tok::Kind::Caret, start, loc);
}

Token Lexer::lexTilde() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('=')) return makeToken(tok::Kind::TildeEqual, start, loc);
  return makeToken(tok::Kind::Tilde, start, loc);
}

Token Lexer::lexQuestion() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  if (consumeIf('?')) return makeToken(tok::Kind::QuestionQuestion, start, loc);
  return makeToken(tok::Kind::Question, start, loc);
}

Token Lexer::lexHash() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  advanceByte();
  return makeToken(tok::Kind::Hash, start, loc);
}


Token Lexer::lexOperatorOrPunctuation() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();

  switch (currentByte()) {
  case '+': return lexPlus();
  case '-': return lexMinus();
  case '*': return lexStar();
  case '/': return lexSlash();
  case '%': return lexPercent();
  case '=': return lexEqual();
  case '!': return lexBang();
  case '<': return lexLess();
  case '>': return lexGreater();
  case '&': return lexAmpersand();
  case '|': return lexPipe();
  case '^': return lexCaret();
  case '~': return lexTilde();
  case '?': return lexQuestion();
  case '.': return lexDot();
  case '#': return lexHash();
  case ':':
    advanceByte();
    if (consumeIf(':')) return makeToken(tok::Kind::DoubleColon, start, loc);
    return makeToken(tok::Kind::Colon, start, loc);
  case '(':
    advanceByte(); return makeToken(tok::Kind::OpenParen, start, loc);
  case ')':
    advanceByte(); return makeToken(tok::Kind::CloseParen, start, loc);
  case '{':
    advanceByte(); return makeToken(tok::Kind::OpenBrace, start, loc);
  case '}':
    advanceByte(); return makeToken(tok::Kind::CloseBrace, start, loc);
  case '[':
    advanceByte(); return makeToken(tok::Kind::OpenBracket, start, loc);
  case ']':
    advanceByte(); return makeToken(tok::Kind::CloseBracket, start, loc);
  case ',':
    advanceByte(); return makeToken(tok::Kind::Comma, start, loc);
  case ';':
    advanceByte(); return makeToken(tok::Kind::Semicolon, start, loc);
  case '\\':
    advanceByte(); return makeToken(tok::Kind::Backslash, start, loc);
  default:
    advanceByte();
    return makeToken(tok::Kind::UnknownOperator, start, loc);
  }
}

Token Lexer::lexUnknown() {
  const std::size_t start = cursor_;
  const SourceLocation loc = currentLocation();
  const unicode::DecodeResult result = unicode::decode(source_, cursor_);

  if (!result.valid) {
    if (options_.diagnoseUnknownCharacters)
      error(loc, "invalid UTF-8 byte in source");
    advanceByte();
    return makeToken(tok::Kind::Unknown, start, loc);
  }

  if (options_.diagnoseUnknownCharacters)
    error(loc, "unexpected character in HyperLang source");

  advanceBytes(result.width);
  return makeToken(tok::Kind::Unknown, start, loc);
}


bool Lexer::scanIdentifier() {
  if (!scanIdentifierContinuation())
    return false;

  while (!atEnd()) {
    if (isAsciiIdentifierContinue(currentByte())) {
      advanceByte();
      continue;
    }

    if (static_cast<unsigned char>(currentByte()) < 0x80u)
      break;

    if (!options_.allowUnicodeIdentifiers)
      break;

    const unicode::DecodeResult result = unicode::decode(source_, cursor_);
    if (!result.valid || !unicode::isIdentifierContinue(result.codePoint))
      break;

    advanceBytes(result.width);
  }

  return true;
}

bool Lexer::scanIdentifierContinuation() {
  if (atEnd())
    return false;

  if (isAsciiIdentifierStart(currentByte())) {
    advanceByte();
    return true;
  }

  if (options_.allowUnicodeIdentifiers &&
      static_cast<unsigned char>(currentByte()) >= 0x80u) {
    const unicode::DecodeResult result = unicode::decode(source_, cursor_);
    if (result.valid && unicode::isIdentifierStart(result.codePoint)) {
      advanceBytes(result.width);
      return true;
    }
  }

  return false;
}

bool Lexer::scanDigits(unsigned base, bool requireDigit) {
  const std::size_t start = cursor_;
  bool previousWasDigit = false;

  while (!atEnd()) {
    const char ch = currentByte();
    if (isDigitForBase(ch, base)) {
      advanceByte();
      previousWasDigit = true;
      continue;
    }

    if (ch == '_') {
      if (!previousWasDigit || !isDigitForBase(peekByte(), base))
        break;
      advanceByte();
      previousWasDigit = false;
      continue;
    }

    break;
  }

  if (requireDigit && cursor_ == start)
    return false;
  return cursor_ != start;
}

bool Lexer::scanExponent() {
  if (currentByte() != 'e' && currentByte() != 'E' &&
      currentByte() != 'p' && currentByte() != 'P')
    return false;

  advanceByte();
  if (currentByte() == '+' || currentByte() == '-')
    advanceByte();

  const std::size_t digitStart = cursor_;
  scanDigits(10, true);
  if (cursor_ == digitStart) {
    error(currentLocation(), "expected exponent digits");
    return false;
  }
  return true;
}

bool Lexer::scanFloatSuffix() {
  if (currentByte() != 'f' && currentByte() != 'F')
    return false;
  if (isAsciiIdentifierContinue(peekByte()))
    return false;
  advanceByte();
  return true;
}

bool Lexer::isDigitForBase(char ch, unsigned base) {
  const unsigned value = asciiDigitValue(static_cast<unsigned char>(ch));
  return value < base;
}

unsigned Lexer::digitValue(char ch) {
  return asciiDigitValue(static_cast<unsigned char>(ch));
}

bool Lexer::isAsciiIdentifierStart(char ch) {
  return isASCIIIdentifierStartByte(static_cast<unsigned char>(ch));
}

bool Lexer::isAsciiIdentifierContinue(char ch) {
  return isASCIIIdentifierContinueByte(static_cast<unsigned char>(ch));
}

std::string Lexer::decodeIdentifier(std::string_view text) {
  return std::string(text);
}

std::string_view Lexer::stripNumericSeparators(std::string_view text) {
  return text;
}


static bool validateASCIIByteForSource(unsigned char byte) noexcept {
  if (byte == 0)
    return false;
  if (byte >= 0x80u)
    return true;
  return !isASCIIControlByte(byte) || isASCIIWhitespaceByte(byte);
}

static bool isPotentialIdentifierContinuation(std::string_view source,
                                              std::size_t offset) noexcept {
  if (offset >= source.size())
    return false;

  const unsigned char byte =
      static_cast<unsigned char>(source[offset]);

  if (byte < 0x80u)
    return isASCIIIdentifierContinueByte(byte);

  const unicode::DecodeResult result = unicode::decode(source, offset);
  return result.valid && unicode::isIdentifierContinue(result.codePoint);
}

static bool isPotentialIdentifierStart(std::string_view source,
                                       std::size_t offset) noexcept {
  if (offset >= source.size())
    return false;

  const unsigned char byte =
      static_cast<unsigned char>(source[offset]);

  if (byte < 0x80u)
    return isASCIIIdentifierStartByte(byte);

  const unicode::DecodeResult result = unicode::decode(source, offset);
  return result.valid && unicode::isIdentifierStart(result.codePoint);
}

static bool isNumericContinuation(std::string_view source,
                                  std::size_t offset) noexcept {
  if (offset >= source.size())
    return false;

  const unsigned char byte =
      static_cast<unsigned char>(source[offset]);

  if (byte < 0x80u)
    return isASCIIDecimalDigitByte(byte) || byte == '_';

  return false;
}

static bool isOperatorLead(unsigned char byte) noexcept {
  return byte < 0x80u && isASCIIOperatorByte(byte);
}

static bool isQuoteLead(unsigned char byte) noexcept {
  return byte < 0x80u && isASCIIQuoteByte(byte);
}

static bool isPunctuationLead(unsigned char byte) noexcept {
  return byte < 0x80u && isASCIIPunctuationByte(byte);
}

Token Lexer::lexImpl() {
  skipTrivia();

  if (atEnd())
    return makeEOFToken();

  if (options_.retainComments && lexer::isCommentStart(source_, cursor_))
    return lexComment();

  const std::size_t tokenOffset = cursor_;
  const SourceLocation tokenLocation = currentLocation();
  const unsigned char byte = static_cast<unsigned char>(currentByte());

  switch (currentByte()) {
  case '\0':
    error(tokenLocation, "embedded null character in source");
    advanceByte();
    return makeToken(tok::Kind::Unknown, tokenOffset, tokenLocation);
  case '@':
    return lexAttributedName();
  case '`':
    if (options_.allowEscapedIdentifiers)
      return lexEscapedIdentifier();
    return lexUnknown();
  case '"':
    return lexString();
  case '\'':
    return lexCharacter();
  default:
    break;
  }

  if (isASCIIDigit(currentByte()))
    return lexNumber();

  if (isAsciiIdentifierStart(currentByte()))
    return lexIdentifierOrKeyword();

  if (options_.allowUnicodeIdentifiers && byte >= 0x80u) {
    const unicode::DecodeResult result = unicode::decode(source_, cursor_);
    if (!result.valid) {
      error(tokenLocation, "invalid UTF-8 sequence in source");
      advanceByte();
      return makeToken(tok::Kind::Unknown, tokenOffset, tokenLocation);
    }
    if (unicode::isIdentifierStart(result.codePoint))
      return lexIdentifierOrKeyword();
  }

  if (byte < 0x80u && isASCIIPunctuationByte(byte))
    return lexOperatorOrPunctuation();

  return lexUnknown();
}

} // namespace hyperlang