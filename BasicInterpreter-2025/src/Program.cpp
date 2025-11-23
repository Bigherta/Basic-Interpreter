// TODO: Imply interfaces declared in the Program.hpp.
#include "../include/Program.hpp"

void Program::addStmt(int line, Statement *stmt) { recorder_.add(line, stmt); }

void Program::removeStmt(int line) { recorder_.remove(line); }

void Program::run()
{
    int index = 0;
    while (index != -1)
    {
        index = recorder_.nextLine(index);
        const Statement *executable = recorder_.get(index);
        executable->execute(vars_, *this);
        delete executable;
    }
    resetAfterRun();
}

void Program::list() const { recorder_.printLines(); }

void Program::clear()
{
    recorder_.clear();
    vars_.clear();
}

void Program::execute(Statement *stmt) { stmt->execute(vars_, (*this)); }

int Program::getPC() const noexcept { return programCounter_; }

void Program::changePC(int line) { programCounter_ = line; }

void Program::programEnd() { programEnd_ = true; }

VarState &Program::get_vars() { return vars_; }

void Program::resetAfterRun() noexcept { clear(); }
