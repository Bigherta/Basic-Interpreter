#pragma once

#include <memory>
#include <string>

class VarState;

class Expression
{
public:
    virtual ~Expression() = default;
    virtual int evaluate(const VarState &state) const = 0; // 返回值的纯虚函数
};

class ConstExpression : public Expression
{
public:
    explicit ConstExpression(int value);
    ~ConstExpression() = default;
    int evaluate(const VarState &state) const override;

private:
    int value_; // 记录常量值
};

class VariableExpression : public Expression
{
public:
    explicit VariableExpression(std::string name);
    ~VariableExpression() = default;
    int evaluate(const VarState &state) const override;

private:
    std::string name_;
};

class CompoundExpression : public Expression
{
public:
    CompoundExpression(Expression *left, char op, Expression *right);
    ~CompoundExpression();
    int evaluate(const VarState &state) const override;

private:
    Expression *left_;
    Expression *right_;
    char op_;
};
