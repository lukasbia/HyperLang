#include <string>
#include <vector>

namespace hyper::ast {

struct ConcurrencyAttribute {
    std::string name;
    bool async = false;
    bool actorIsolated = false;
    bool sendable = false;
};

struct TaskExpression {
    std::string priority;
    void* body = nullptr;
    bool detached = false;
};

struct AwaitExpression {
    void* expression = nullptr;
};

bool isAsync(const ConcurrencyAttribute& attribute) {
    return attribute.async;
}

bool isActorIsolated(const ConcurrencyAttribute& attribute) {
    return attribute.actorIsolated;
}

bool isDetachedTask(const TaskExpression& task) {
    return task.detached;
}

} // namespace hyper::ast
