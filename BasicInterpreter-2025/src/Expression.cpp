#include "../include/Expression.hpp"
#include <memory>

#include "../include/VarState.hpp"
#include "../include/utils/Error.hpp"

ConstExpression::ConstExpression(int value) : value_(value) {} // 常量的构造函数

int ConstExpression::evaluate(const std::vector<VarState> &state) const { return value_; } // 常量表达式的值就是常量本身

VariableExpression::VariableExpression(std::string name) : name_(std::move(name)) {} // 移动构造函数

int VariableExpression::evaluate(const std::vector<VarState> &state) const
{
    for (int i = state.size() - 1; i >= 0; i--)
    {
        if (state[i].get_map().count(name_))
        {
            return state[i].getValue(name_);
        }
    }
    throw BasicError("VARIABLE NOT DEFINED");
}

CompoundExpression::CompoundExpression(std::shared_ptr<Expression> left, char op, std::shared_ptr<Expression> right) :
    left_(left), right_(right), op_(op)
{
    return;
} // 复合表达式的构造函数

int CompoundExpression::evaluate(const std::vector<VarState> &state) const
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
