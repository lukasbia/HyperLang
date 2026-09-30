#pragma once
#include <string>
namespace hyperlang::Decimal { struct Value { long double value=0; explicit Value(long double v=0):value(v){} std::string text()const{return std::to_string(value);} }; }
