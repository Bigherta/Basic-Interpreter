// TODO: Imply interfaces declared in the Program.hpp.
#include "../include/Program.hpp"
#include <iostream>
Program::Program() : vars_(), recorder_(), programCounter_(1), programEnd_(false) { return; }

void Program::addStmt(int line, Statement *stmt) { recorder_.add(line, stmt); }

void Program::removeStmt(int line) { recorder_.remove(line); }

void Program::run()
{
    
    programCounter_ = recorder_.nextLine(0);
    while (programCounter_ != -1)
    {
        int pre_programcounter = programCounter_;
        const Statement *executable = recorder_.get(programCounter_);
        executable->execute(vars_, *this);
        if (programEnd_) break;
        if (pre_programcounter == programCounter_)
            programCounter_ = recorder_.nextLine(programCounter_);
    }
    programEnd_ = false;
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

bool Program::hasline(int line) { return recorder_.hasLine(line); }

void Program::resetAfterRun() noexcept { return; }
