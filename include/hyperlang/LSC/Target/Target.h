#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace hyperlang::lsc::target {
enum class Architecture { X86, X86_64, ARM, ARM64, ARM64EC, RISC_V32, RISC_V64, MIPS, MIPS64, PowerPC, PowerPC64, SPARC, SPARC64, S390X, AVR, MSP430, WASM32, WASM64 };
enum class OperatingSystem { Unknown, Linux, Windows, Darwin, FreeBSD, OpenBSD, NetBSD, Android, IOS, MacOS, Wasm }; 
enum class ObjectFormat { ELF, COFF, MachO, Wasm }; 
struct Target { Architecture architecture; OperatingSystem operatingSystem; ObjectFormat objectFormat; uint32_t pointerWidth; std::string triple; std::vector<std::string> features; };
Target parseTarget(const std::string& triple);
Target hostTarget();
bool supportsTarget(Architecture architecture);
std::string architectureName(Architecture architecture);
std::string operatingSystemName(OperatingSystem operatingSystem);
std::string objectFormatName(ObjectFormat format);
}
