#include "value.hpp"

Value::Value(Type type, std::string content) : type(type), content(content) {}
Value::Value(Type type) : type(type), content("") {}
Value::Value() : type(Type::Void), content("") {}