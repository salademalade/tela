#include "token.hpp"

Token::Token(Type type) : type(type), value("") {}

Token::Token(Type type, std::string value) : type(type), value(value) {}
