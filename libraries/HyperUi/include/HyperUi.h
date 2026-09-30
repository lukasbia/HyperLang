#pragma once
#include <string>
#include <utility>
namespace hyperlang::HyperUi { struct Text { std::string value; explicit Text(std::string v):value(std::move(v)){} }; }
