#include "../include/Statement.hpp"
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <utility>
#include "../include/Lexer.hpp"
#include "../include/Parser.hpp"
#include "../include/Program.hpp"
#include "../include/VarState.hpp"
#include "../include/utils/Error.hpp"
Statement::Statement(std::string source) : source_(std::move(source)) {}

const std::string &Statement::text() const noexcept { return source_; }

GOTOstatement::GOTOstatement(std::string source, int targetline) : Statement(source) { gotoPC = targetline; }

void GOTOstatement::execute(VarState &state, Program &program) const { program.changePC(gotoPC); }

PrintStatement::PrintStatement(std::string source, Expression *expression) : Statement(source) { exp = expression; }

void PrintStatement::execute(VarState &state, Program &program) const { std::cout << exp->evaluate(state) << '\n'; }

LetStatement::LetStatement(std::string source, Expression *expression) : Statement(source) { exp = expression; }

void LetStatement::execute(VarState &state, Program &program) const
{
    Lexer temp;
    TokenStream tokens = temp.tokenize(source_);
    tokens.get();
    state.setValue((tokens.get())->text, exp->evaluate(state));
}

InputStatement::InputStatement(std::string source, std::string name) : Statement(source) { var_name = name; }

void InputStatement::execute(VarState &state, Program &program) const
{
    int x;
    std::cout << "?";
    std::cin >> x;
    state.setValue(var_name, x);
}

RemStatement::RemStatement(std::string source) : Statement(source) { return; }

void RemStatement::execute(VarState &state, Program &program) const { program.removeStmt(program.getPC()); }

EndStatement::EndStatement(std::string source) : Statement(source) { return; }

void EndStatement::execute(VarState &state, Program &program) const
{
    state.clear();
    program.clear();
}

IfStatement::IfStatement(std::string source, int targetline, Expression *l, Expression *r, char o) :
    GOTOstatement(source, targetline)
{
    left = l;
    right = r;
    op = o;
}

void IfStatement::execute(VarState &state, Program &program) const
{
    int val_l = left->evaluate(state);
    int val_r = right->evaluate(state);
    if ((op == '=' && val_l == val_r) || (op == '<' && val_l < val_r) || (op == '>' && val_l > val_r))
    {
        GOTOstatement::execute(state, program);
    }
}
