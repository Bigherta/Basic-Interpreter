#pragma once

#include <memory>
#include <string>
#include <vector>

class VarState;

class Expression
{
public:
    virtual int evaluate(const std::vector<VarState> &state) const = 0; // 返回值的纯虚函数
};

class ConstExpression : public Expression
{
public:
    explicit ConstExpression(int value);

    int evaluate(const std::vector<VarState> &state) const override;

private:
    int value_; // 记录常量值
};

class VariableExpression : public Expression
{
public:
    explicit VariableExpression(std::string name);

    int evaluate(const std::vector<VarState> &state) const override;

private:
    std::string name_;
};

class CompoundExpression : public Expression
{
public:
    CompoundExpression(std::shared_ptr<Expression> left, char op, std::shared_ptr<Expression> right);

    int evaluate(const std::vector<VarState> &state) const override;

private:
    std::shared_ptr<Expression> left_;
    std::shared_ptr<Expression> right_;
    char op_;
};
