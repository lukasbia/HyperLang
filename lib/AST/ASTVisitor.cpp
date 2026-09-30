#include <cstddef>
#include <vector>

namespace hyper::ast {

class Node {
public:
    virtual ~Node() = default;
};

class Visitor {
public:
    virtual ~Visitor() = default;

    virtual bool visit(Node&) {
        return true;
    }

    virtual void leave(Node&) {
    }

    void walk(Node& node) {
        if (!visit(node)) {
            return;
        }
        leave(node);
    }
};

class ConstVisitor {
public:
    virtual ~ConstVisitor() = default;

    virtual bool visit(const Node&) {
        return true;
    }

    virtual void leave(const Node&) {
    }

    void walk(const Node& node) {
        if (!visit(node)) {
            return;
        }
        leave(node);
    }
};

} // namespace hyper::ast
