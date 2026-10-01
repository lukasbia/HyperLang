#pragma once

namespace hyper {

struct ParserOptions {
    bool allowTopLevelCode = true;
    bool parseGenerics = true;
    bool parseAttributes = true;
    bool recoverFromErrors = true;
};

} // namespace hyper
