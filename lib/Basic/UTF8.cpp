//===--- UTF8.cpp - HyperLang UTF-8 Utilities -----------------------------===//

#include "hyperlang/Basic/UTF8.h"

namespace hyperlang::unicode {

bool isContinuationByte(unsigned char byte) {
  return (byte & 0xC0u) == 0x80u;
}

bool isValidScalar(std::uint32_t cp) {
  return cp <= 0x10FFFFu && !(cp >= 0xD800u && cp <= 0xDFFFu);
}

DecodeResult decode(std::string_view text, std::size_t offset) {
  if (offset >= text.size())
    return {0, 0, false};

  const auto b0 = static_cast<unsigned char>(text[offset]);
  if (b0 < 0x80u)
    return {b0, 1, true};

  if (b0 >= 0xC2u && b0 <= 0xDFu) {
    if (offset + 1 >= text.size())
      return {0, 1, false};
    const auto b1 = static_cast<unsigned char>(text[offset + 1]);
    if (!isContinuationByte(b1))
      return {0, 1, false};
    return {((b0 & 0x1Fu) << 6) | (b1 & 0x3Fu), 2, true};
  }

  if (b0 >= 0xE0u && b0 <= 0xEFu) {
    if (offset + 2 >= text.size())
      return {0, 1, false};
    const auto b1 = static_cast<unsigned char>(text[offset + 1]);
    const auto b2 = static_cast<unsigned char>(text[offset + 2]);
    if (!isContinuationByte(b1) || !isContinuationByte(b2))
      return {0, 1, false};
    if (b0 == 0xE0u && b1 < 0xA0u)
      return {0, 1, false};
    if (b0 == 0xEDu && b1 >= 0xA0u)
      return {0, 1, false};
    const std::uint32_t cp =
        ((b0 & 0x0Fu) << 12) | ((b1 & 0x3Fu) << 6) | (b2 & 0x3Fu);
    return {cp, 3, isValidScalar(cp)};
  }

  if (b0 >= 0xF0u && b0 <= 0xF4u) {
    if (offset + 3 >= text.size())
      return {0, 1, false};
    const auto b1 = static_cast<unsigned char>(text[offset + 1]);
    const auto b2 = static_cast<unsigned char>(text[offset + 2]);
    const auto b3 = static_cast<unsigned char>(text[offset + 3]);
    if (!isContinuationByte(b1) || !isContinuationByte(b2) ||
        !isContinuationByte(b3))
      return {0, 1, false};
    if (b0 == 0xF0u && b1 < 0x90u)
      return {0, 1, false};
    if (b0 == 0xF4u && b1 > 0x8Fu)
      return {0, 1, false};
    const std::uint32_t cp =
        ((b0 & 0x07u) << 18) | ((b1 & 0x3Fu) << 12) |
        ((b2 & 0x3Fu) << 6) | (b3 & 0x3Fu);
    return {cp, 4, isValidScalar(cp)};
  }

  return {0, 1, false};
}

bool isIdentifierStart(std::uint32_t cp) {
  return (cp == '_' || (cp >= 'A' && cp <= 'Z') ||
          (cp >= 'a' && cp <= 'z') ||
          (cp >= 0x80u && isValidScalar(cp)));
}

bool isIdentifierContinue(std::uint32_t cp) {
  return isIdentifierStart(cp) || (cp >= '0' && cp <= '9');
}

bool isWhitespace(std::uint32_t cp) {
  switch (cp) {
  case 0x09: case 0x0A: case 0x0B: case 0x0C: case 0x0D:
  case 0x20: case 0x85: case 0xA0: case 0x1680:
  case 0x2000: case 0x2001: case 0x2002: case 0x2003:
  case 0x2004: case 0x2005: case 0x2006: case 0x2007:
  case 0x2008: case 0x2009: case 0x200A: case 0x2028:
  case 0x2029: case 0x202F: case 0x205F: case 0x3000:
    return true;
  default:
    return false;
  }
}

std::size_t encodedLength(std::uint32_t cp) {
  if (!isValidScalar(cp)) return 0;
  if (cp < 0x80u) return 1;
  if (cp < 0x800u) return 2;
  if (cp < 0x10000u) return 3;
  return 4;
}

bool validate(std::string_view text) {
  std::size_t offset = 0;
  while (offset < text.size()) {
    const DecodeResult result = decode(text, offset);
    if (!result.valid || result.width == 0) return false;
    offset += result.width;
  }
  return true;
}

} // namespace hyperlang::unicode
