#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

namespace hyper::ast {

class Node {
public:
    virtual ~Node() = default;
};

class ASTContext {
public:
    ASTContext() = default;
    ASTContext(const ASTContext&) = delete;
    ASTContext& operator=(const ASTContext&) = delete;

    template <typename T, typename... Arguments>
    T* create(Arguments&&... arguments) {
        auto node = std::make_unique<T>(
            std::forward<Arguments>(arguments)...
        );
        T* result = node.get();
        nodes_.push_back(std::move(node));
        return result;
    }

    void clear() {
        nodes_.clear();
    }

    std::size_t size() const noexcept {
        return nodes_.size();
    }

private:
    std::vector<std::unique_ptr<Node>> nodes_;
};

} // namespace hyper::ast
