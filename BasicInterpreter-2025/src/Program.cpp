// TODO: Imply interfaces declared in the Program.hpp.
#include "../include/Program.hpp"

void Program::addStmt(int line, Statement *stmt)
{
    recorder_.add(line, stmt);
}

void Program::removeStmt(int line)
{
    recorder_.remove(line);
}

void Program::run()
{
    int index = 0;
    while (index != -1)
    {
        index = recorder_.nextLine(index);
        const Statement *executable = recorder_.get(index);
        //to do (判断不同派生类并构造对应引用进行执行)
    }
}

void Program::list() const
{
    recorder_.printLines();
}

void Program::clear()
{
    recorder_.clear();
    vars_.clear();
}

void execute(Statement *stmt)
{
    //to do 
}

int Program::getPC() const noexcept
{
    return programCounter_;
}

void Program::changePC(int line)
{
    programCounter_ = line;
}

void Program::programEnd()
{
    programEnd_ = true;
}

void Program::resetAfterRun() noexcept
{
    //to do
}