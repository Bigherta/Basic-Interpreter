#include "../include/Lexer.hpp"

#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

#include "../include/utils/Error.hpp"

const std::unordered_map<std::string, TokenType> TABLE = {
        {"LET", TokenType::LET},  {"PRINT", TokenType::PRINT}, {"INPUT", TokenType::INPUT}, {"END", TokenType::END},
        {"REM", TokenType::REM},  {"GOTO", TokenType::GOTO},   {"IF", TokenType::IF},       {"THEN", TokenType::THEN},
        {"RUN", TokenType::RUN},  {"LIST", TokenType::LIST},   {"CLEAR", TokenType::CLEAR}, {"QUIT", TokenType::QUIT},
        {"HELP", TokenType::HELP}}; // 构建从字符串到枚举类的一个映射

bool isOverflow(const std::string &digits, bool negative)
{
    constexpr long long max_limit = std::numeric_limits<int>::max();
    return negative ? std::stol(digits) > max_limit : std::stol(digits) > max_limit + 1;
} // 判断数字是否溢出

TokenStream Lexer::tokenize(const std::string &line) const // 解析一行源码
{
    std::vector<Token> tokens;
    int column = 0;
    while (column < line.size())
    {
        char ch = line[column];
        // 如果ch是空格
        if (std::isspace(static_cast<int>(ch)))
        {
            ++column;
            continue;
        }
        // 如果ch是字母
        if (isLetterChar(ch))
        {
            int start = column;
            ++column;
            while (column < line.size() && isLetterChar(line[column]))
            {
                ++column;
            }
            std::string text = line.substr(start, column - start); // 提取首个单词
            TokenType type = matchKeyword(text); // 判断是哪一个枚举类型
            switch (type)
            {
                case TokenType::REM:
                    tokens.push_back(Token{TokenType::REM, text, column}); // 封装指令REM
                    if (column < line.size())
                    {
                        std::string comment = line.substr(column);
                        tokens.push_back(Token{TokenType::REMINFO, comment, column + 1}); // 封装注释文本
                    }
                    return TokenStream(std::move(tokens));
                case TokenType::UNKNOWN:
                    tokens.push_back(Token{TokenType::IDENTIFIER, text, column}); // 封装新的变量名
                    break;
                default:
                    tokens.push_back(Token{type, text, column}); // 插入进Token封装成对应的指令
            }
            continue;
        }
        // 如果ch是数字
        if (isNumberChar(ch))
        {
            int start = column;
            while (column < line.size() && isNumberChar(line[column]))
            {
                ++column;
            }
            std::string text = line.substr(start, column - start);
            tokens.push_back(Token{TokenType::NUMBER, text, column}); // 封装一个数字
            continue;
        }

        TokenType symbolType = TokenType::UNKNOWN;
        switch (ch)
        {
            case '+':
                symbolType = TokenType::PLUS;
                break;
            case '-':
                symbolType = TokenType::MINUS;
                break;
            case '*':
                symbolType = TokenType::MUL;
                break;
            case '/':
                symbolType = TokenType::DIV;
                break;
            case '=':
                symbolType = TokenType::EQUAL;
                break;
            case '>':
                symbolType = TokenType::GREATER;
                break;
            case '<':
                symbolType = TokenType::LESS;
                break;
            case '(':
                symbolType = TokenType::LEFT_PAREN;
                break;
            case ')':
                symbolType = TokenType::RIGHT_PAREN;
                break;
            case ',':
                symbolType = TokenType::COMMA;
                break;
            default:
                break;
        }
        if (symbolType != TokenType::UNKNOWN)
        {
            tokens.push_back(Token{symbolType, std::string(1, ch), column});
            ++column;
            continue;
        } // 封装一个符号

        throw BasicError("Unexpected character '" + std::string(1, ch) + "' at column " +
                         std::to_string(column)); // 异常抛出
    }
    return TokenStream(std::move(tokens));
}

bool Lexer::isLetterChar(char ch) noexcept { return std::isalpha(static_cast<unsigned char>(ch)); }

bool Lexer::isNumberChar(char ch) noexcept { return std::isalnum(static_cast<unsigned char>(ch)) || ch == '_'; }

TokenType Lexer::matchKeyword(const std::string &text) noexcept
{
    auto it = TABLE.find(text);
    if (it != TABLE.end())
    {
        return it->second;
    }
    return TokenType::UNKNOWN;
}
