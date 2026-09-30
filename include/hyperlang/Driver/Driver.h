#pragma once
#include <string>
#include <vector>
namespace hyperlang::driver { struct DriverOptions { bool optimize = true; bool emitHIL = false; bool emitLSCIR = false; }; class Driver { public: int run(const std::string& inputPath, const DriverOptions& options = {}); private: int compileSource(const std::string&, const DriverOptions&); }; }
