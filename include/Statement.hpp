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

    const std::string &text() const noexcept; // 返回该行语句

protected:
    std::string source_; // 储存一行语句
};

class GOTOstatement : public Statement
{
private:
    int gotoPC;

public:
    GOTOstatement(std::string source, int targetline);
    void execute(VarState &state, Program &program) const override;
};
class PrintStatement : public Statement
{
private:
    Expression *exp;

public:
    PrintStatement(std::string source, Expression *expression);
    ~PrintStatement() { delete exp; }
    void execute(VarState &state, Program &program) const override;
};

class LetStatement : public Statement
{
private:
    Expression *exp;

public:
    LetStatement(std::string source, Expression *expression);
    ~LetStatement() { delete exp; }
    void execute(VarState &state, Program &program) const override;
};

class InputStatement : public Statement
{
private:
    std::string var_name;

public:
    InputStatement(std::string source, std::string name);
    void execute(VarState &state, Program &program) const override;
};

class RemStatement : public Statement
{
public:
    RemStatement(std::string source);
    void execute(VarState &state, Program &program) const override;
};

class EndStatement : public Statement
{
public:
    EndStatement(std::string source);
    void execute(VarState &state, Program &program) const override;
};

class IfStatement : public GOTOstatement
{
private:
    Expression *left, *right;
    char op;

public:
    IfStatement(std::string source, int target, Expression *l, Expression *r, char o);
    ~IfStatement()
    {
        delete left;
        delete right;
    }
    void execute(VarState &state, Program &program) const override;
};
