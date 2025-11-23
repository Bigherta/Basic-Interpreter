#include "../include/Expression.hpp"

#include "../include/VarState.hpp"
#include "../include/utils/Error.hpp"

ConstExpression::ConstExpression(int value) : value_(value) {} // 常量的构造函数

int ConstExpression::evaluate(const VarState &) const { return value_; } // 常量表达式的值就是常量本身

VariableExpression::VariableExpression(std::string name) : name_(std::move(name)) {} // 移动构造函数

int VariableExpression::evaluate(const VarState &state) const
{
    return state.getValue(name_); // 返回变量名对应的变量值
}

CompoundExpression::CompoundExpression(Expression *left, char op, Expression *right) :
    left_(left), right_(right), op_(op)
{
} // 复合表达式的构造函数

CompoundExpression::~CompoundExpression()
{
    delete left_;
    delete right_;
} // 复合表达式的析构

int CompoundExpression::evaluate(const VarState &state) const
{
    int lhs = left_->evaluate(state);
    int rhs = right_->evaluate(state);

    switch (op_)
    {
        case '+':
            return lhs + rhs;
        case '-':
            return lhs - rhs;
        case '*':
            return lhs * rhs;
        case '/':
            if (rhs == 0)
            {
                throw BasicError("DIVIDE BY ZERO");
            }
            return lhs / rhs;
        default:
            throw BasicError("UNSUPPORTED OPERATOR");
    }
} // 利用树状结构实现复合表达式的值的返回与异常抛出
