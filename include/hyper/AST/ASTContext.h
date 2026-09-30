#pragma once

#include <cstddef>
#include <memory>
#include <vector>

namespace hyper::ast {

class Node;

class ASTContext {
public:
    ASTContext();
    ASTContext(const ASTContext&) = delete;
    ASTContext& operator=(const ASTContext&) = delete;
    ASTContext(ASTContext&&) noexcept = default;
    ASTContext& operator=(ASTContext&&) noexcept = default;
    ~ASTContext();

    template <typename T, typename... Arguments>
    T* create(Arguments&&... arguments);

    void clear();
    std::size_t size() const noexcept;

private:
    std::vector<std::unique_ptr<Node>> nodes_;
};

} // namespace hyper::ast
