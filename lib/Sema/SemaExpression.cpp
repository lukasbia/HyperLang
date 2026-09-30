#include "hyper/Sema/Sema.h"
#include <string_view>

namespace hyper::sema {

bool validateBinaryOperator(std::string_view operation) {
    return operation == "+" || operation == "-" || operation == "*" ||
           operation == "/" || operation == "%" || operation == "==" ||
           operation == "!=" || operation == "<" || operation == ">" ||
           operation == "<=" || operation == ">=" || operation == "&&" ||
           operation == "||" || operation == "=";
}

bool validateUnaryOperator(std::string_view operation) {
    return operation == "!" || operation == "-" || operation == "+" ||
           operation == "~";
}

} // namespace hyper::sema
