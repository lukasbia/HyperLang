#include "hyper/Parser.h"
#include <string>
#include <vector>

namespace hyper::parse {

struct ParseRequest {
    std::string name;
    std::string payload;
};

class ParseRequestQueue {
public:
    void push(ParseRequest request) {
        requests_.push_back(std::move(request));
    }

    bool empty() const {
        return requests_.empty();
    }

    std::size_t size() const {
        return requests_.size();
    }

private:
    std::vector<ParseRequest> requests_;
};

bool isParseRequest(const std::string& spelling) {
    return spelling == "import" || spelling == "#if" || spelling == "#sourceLocation";
}

} // namespace hyper::parse
