#include <string>
#include <vector>

namespace hyper::ast {

struct ClosureParameter {
    std::string name;
    std::string type;
};

struct ClosureExpression {
    std::vector<ClosureParameter> parameters;
    std::vector<std::string> captures;
    void* body = nullptr;
    bool escaping = false;
    bool async = false;
};

bool hasParameters(const ClosureExpression& closure) {
    return !closure.parameters.empty();
}

bool hasCaptures(const ClosureExpression& closure) {
    return !closure.captures.empty();
}

bool isEscaping(const ClosureExpression& closure) {
    return closure.escaping;
}

} // namespace hyper::ast
