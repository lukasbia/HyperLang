#pragma once
#include <string>
namespace hyperlang::HyperUi { struct Text { std::string value; explicit Text(std::string v):value(std::move(v)){} }; }
