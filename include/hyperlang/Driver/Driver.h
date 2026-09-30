#pragma once

#include <string>

namespace hyperlang::driver {

struct DriverOptions {
    bool optimize = true;
    bool emitHIL = false;
    bool emitLSCIR = false;
    bool emitMachineCode = false;
    bool emitAssembly = false;
    bool emitHex = false;
    bool link = true;
    std::string target = "generic";
    std::string outputPath = "a.out";
};

class Driver {
public:
    int run(const std::string &inputPath,
            const DriverOptions &options = {});

private:
    int compileSource(const std::string &source,
                      const DriverOptions &options);
};

} // namespace hyperlang::driver
