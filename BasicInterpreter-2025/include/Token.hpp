#pragma once

#include <memory>
#include <string>
#include <vector>

enum class TokenType
{
    // 关键字
    LET, // 赋值
    PRINT, // 输出
    INPUT, // 输入
    END, // 结束
    REM, // 注释
    GOTO, // 转到某一行
    IF, // 条件
    THEN, // 结果
    RUN, // 运行
    LIST, // 列出所有步骤
    CLEAR, // 删除所有程序与变量
    QUIT, // 调用exit(0)来退出解释器
    HELP, // 描述你的解释器
          // 基础语法单元
    IDENTIFIER, // 命名标识
    NUMBER, // 数字
    REMINFO, // 注释
             // 运算与符号
    PLUS, // 加法
    MINUS, // 减法
    MUL, // 乘法
    DIV, // 除法
    EQUAL, // 相等判断
    GREATER, // 大于判断
    LESS, // 小于判断
    LEFT_PAREN, // 左括号
    RIGHT_PAREN, // 右括号
    COMMA, // 逗号
           // 保留占位
    UNKNOWN,

    INDENT, // 新增作用域
    DEDENT // 退出作用域
}; // 枚举指令类型

struct Token
{
    TokenType type{TokenType::UNKNOWN}; // 封装类型
    std::string text{}; // 封装原始文本（分为关键字、基本语法单元、运算与符号）
    int column{0}; // 追踪当前列
};

class TokenStream
{
public:
    TokenStream() = default; // 默认构造
    explicit TokenStream(std::vector<Token> &&tokens);

    const std::shared_ptr<Token> peek() const;
    const std::shared_ptr<Token> get();
    bool empty() const;
    void reset();

    int position() const;
    int size() const;

    void push(Token &&token);
    const std::vector<Token> &data() const;

private:
    std::vector<Token> tokens_{};
    int cursor_{0};
};
