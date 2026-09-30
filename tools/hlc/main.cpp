#include "hyperlang/Driver/Driver.h"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage: hlc <file.hl>\n";
        return 1;
    }
    hyperlang::driver::DriverOptions options;
    for (int i = 2; i < argc; ++i) {
        const std::string argument = argv[i];
        if (argument == "--no-opt") options.optimize = false;
        else if (argument == "--emit-hil") options.emitHIL = true;
        else if (argument == "--emit-lsc-ir") options.emitLSCIR = true;
        else { std::cerr << "unknown option: " << argument << '\n'; return 1; }
    }
    hyperlang::driver::Driver driver;
    return driver.run(argv[1], options);
}
