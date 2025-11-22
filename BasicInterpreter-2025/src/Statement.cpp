#include "../include/Statement.hpp"

#include <iostream>
#include <limits>
#include <sstream>
#include <utility>

#include "../include/Program.hpp"
#include "../include/VarState.hpp"
#include "../include/utils/Error.hpp"

Statement::Statement(std::string source) : source_(std::move(source)) {}

const std::string &Statement::text() const noexcept { return source_; }

// TODO: Imply interfaces declared in the Statement.hpp.
GOTOstatement::GOTOstatement(std::string source, int PC) : Statement(source) { gotoPC = PC; }
void GOTOstatement::execute(VarState &state, Program &program) { program.changePC(gotoPC); }
