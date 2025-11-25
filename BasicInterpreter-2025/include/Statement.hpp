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

    virtual void execute(std::vector<VarState> &state, Program &program) const = 0;

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
    void execute(std::vector<VarState> &state, Program &program) const override;
};
class PrintStatement : public Statement
{
private:
    std::shared_ptr<Expression> exp;

public:
    PrintStatement(std::string source, std::shared_ptr<Expression> expression);

    void execute(std::vector<VarState> &state, Program &program) const override;
};

class LetStatement : public Statement
{
private:
    std::shared_ptr<Expression> exp;

public:
    LetStatement(std::string source, std::shared_ptr<Expression> expression);

    void execute(std::vector<VarState> &state, Program &program) const override;
};

class InputStatement : public Statement
{
private:
    std::string var_name;

public:
    InputStatement(std::string source, std::string name);
    void execute(std::vector<VarState> &state, Program &program) const override;
};

class RemStatement : public Statement
{
public:
    RemStatement(std::string source);
    void execute(std::vector<VarState> &state, Program &program) const override;
};

class EndStatement : public Statement
{
public:
    EndStatement(std::string source);
    void execute(std::vector<VarState> &state, Program &program) const override;
};

class IfStatement : public GOTOstatement
{
private:
    std::shared_ptr<Expression> left, right;
    char op;

public:
    IfStatement(std::string source, int target, std::shared_ptr<Expression> l, std::shared_ptr<Expression> r, char o);

    void execute(std::vector<VarState> &state, Program &program) const override;
};

class IndentStatement : public Statement
{
public:
    IndentStatement(std::string source);
    void execute(std::vector<VarState> &state, Program &program) const override;
};

class DedentStatement : public Statement
{
public:
    DedentStatement(std::string source);
    void execute(std::vector<VarState> &state, Program &program) const override;
};
