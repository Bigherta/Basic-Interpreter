#pragma once

#include <string>

#include "Token.hpp"

class Lexer
{
public:
    TokenStream tokenize(const std::string &line) const; // 将一行源码进行语义分解

private:
    static bool isLetterChar(char ch) noexcept; // 判断是否是字母
    static bool isNumberChar(char ch) noexcept; // 判断是否是数字
    static TokenType matchKeyword(const std::string &text) noexcept; // 匹配关键字
};
