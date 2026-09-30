#include "hyper/Parser.h"
#include <cstddef>
#include <string>
#include <vector>

namespace hyper::parse {

class PersistentParserState {
public:
    explicit PersistentParserState(std::size_t position = 0)
        : position_(position) {
    }

    std::size_t position() const {
        return position_;
    }

    void setPosition(std::size_t position) {
        position_ = position;
    }

    void pushScope(std::string name) {
        scopes_.push_back(std::move(name));
    }

    void popScope() {
        if (!scopes_.empty()) {
            scopes_.pop_back();
        }
    }

    const std::vector<std::string>& scopes() const {
        return scopes_;
    }

private:
    std::size_t position_;
    std::vector<std::string> scopes_;
};

} // namespace hyper::parse
