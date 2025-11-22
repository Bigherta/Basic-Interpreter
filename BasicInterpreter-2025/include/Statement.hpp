#pragma once

#include <memory>
#include <string>
#include "Expression.hpp"
class Program;
class VarState;

class Statement
{
public:
    explicit Statement(std::string source);
    virtual ~Statement() = default;

    virtual void execute(VarState &state, Program &program) const = 0;

    const std::string &text() const noexcept; //返回该行语句

private:
    std::string source_;//储存一行语句
};

// TODO: Other statement types derived from Statement, e.g., GOTOStatement,
// LetStatement, etc.
class GOTOstatement : public Statement
{
    private:
    int gotoPC;
    public:
    GOTOstatement(std::string, int);
    void execute(VarState &state, Program &program);
};
class PrintStatement : public Statement
{
    public:
    void execute(VarState &state, Program &program);
};
