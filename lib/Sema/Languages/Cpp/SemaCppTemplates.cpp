#include "hyper/Sema/Sema.h"
#include <string_view>
namespace hyper::sema::cpp { bool isTemplateKeyword(std::string_view n) { return n=="template"||n=="typename"||n=="concept"||n=="requires"; } }
