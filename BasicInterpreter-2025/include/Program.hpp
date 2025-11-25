#pragma once

#include <memory>
#include <vector>

#include "Recorder.hpp"
#include "VarState.hpp"

class Statement;

class Program
{
public:
    Program();

    // 添加一行程序。
    void addStmt(int line, std::shared_ptr<Statement> stmt);

    // 删除行号对应的语句。
    void removeStmt(int line);

    // 按行号升序执行程序，支持 GOTO/IF 改变 PC。
    void run();

    // 输出 <line> <stmt>。
    void list() const;

    // 清空 Recorder 与 VarState
    void clear();

    // 执行一条语句
    void execute(Statement *stmt);

    // 获取当前行号，RUN 期间有效
    int getPC() const noexcept;

    // 强制改变 PC，用于 GOTO/IF
    void changePC(int line);

    // 程序结束
    void programEnd();

    // 获取当前所有变量状态
    std::vector<VarState> &get_vars();

    // 程序是否有行line
    bool hasline(int line);

    // 新增作用域
    void indent();

    // 退出作用域
    void dedent();

private:
    Recorder recorder_;
    std::vector<VarState> vars_;
    // 当前行号；RUN 前设为最小行
    int programCounter_;
    bool programEnd_;

    void resetAfterRun() noexcept;
};
