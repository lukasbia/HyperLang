#include <string>
#include <string_view>

namespace hyper::ast {

class Node {
public:
    virtual ~Node() = default;
    virtual std::string kind() const = 0;
};

class ASTPrinter {
public:
    std::string print(const Node& node) const {
        return node.kind();
    }

    std::string_view printKind(const Node& node) const {
        return node.kind();
    }
};

std::string dumpAST(const Node& node) {
    ASTPrinter printer;
    return printer.print(node);
}

} // namespace hyper::ast
