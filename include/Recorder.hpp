#pragma once

#include <map>
#include <memory>
#include <utility>
#include <vector>
#include "Statement.hpp"

class Recorder
{
public:
    ~Recorder();

    // 插入或覆盖指定行。
    void add(int line, Statement *stmt);

    // 删除行，不存在则无事发生。
    void remove(int line);

    // 读取行号对应 Stmt ，不存在则返回 nullptr。
    const Statement *get(int line) const noexcept;

    // 询问行号对应 Stmt 是否存在。
    bool hasLine(int line) const noexcept;

    // 清空全部行。
    void clear() noexcept;

    // 按行号升序输出所有程序。
    void printLines() const;

    // 返回大于 line 的最小行号，不存在则返回 -1。
    int nextLine(int line) const noexcept;


private:
    // TODO.
    std::map<int, Statement *> record;
    int max_PC;
};
