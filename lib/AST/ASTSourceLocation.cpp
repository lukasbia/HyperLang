#include <cstddef>
#include <string>

namespace hyper::ast {

struct SourcePosition {
    std::size_t offset = 0;
    std::size_t line = 1;
    std::size_t column = 1;
};

struct SourceRange {
    SourcePosition begin;
    SourcePosition end;

    bool empty() const noexcept {
        return begin.offset == end.offset;
    }

    std::size_t length() const noexcept {
        return end.offset >= begin.offset
            ? end.offset - begin.offset
            : 0;
    }
};

bool contains(
    const SourceRange& range,
    std::size_t offset
) noexcept {
    return offset >= range.begin.offset &&
           offset <= range.end.offset;
}

bool precedes(
    const SourcePosition& left,
    const SourcePosition& right
) noexcept {
    return left.offset < right.offset;
}

} // namespace hyper::ast
