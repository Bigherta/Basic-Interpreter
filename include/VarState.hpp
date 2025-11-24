#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class VarState
{
public:
    void setValue(const std::string &name, int value); // 赋值
    int getValue(const std::string &name) const; // 获取值
    void clear(); // 删除所有变量

private:
    std::unordered_map<std::string, int> values_; // 用一个无序映射来储存变量名与变量值
};
