#include <iostream>
#include <memory>
#include <string>

#include "../include/Lexer.hpp"
#include "../include/Parser.hpp"
#include "../include/Program.hpp"
#include "../include/Token.hpp"
#include "../include/utils/Error.hpp"

int main()
{
    Lexer lexer;
    Parser parser;
    Program program;

    std::string line;
    while (std::getline(std::cin, line))
    {
        if (line.empty())
        {
            continue;
        }
        try
        {
            TokenStream tokens_of_line = lexer.tokenize(line);
            ParsedLine Aline = parser.parseLine(tokens_of_line, line);
            if (Aline.getLine().has_value())
                program.addStmt(Aline.getLine().value(), Aline.getStatement());
            else
            {
                const TokenType first_token = tokens_of_line.peek()->type;
                switch (first_token)
                {
                    case TokenType::LIST:

                        program.list();
                        break;

                    case TokenType::QUIT:

                        exit(0);
                        break;

                    case TokenType::CLEAR:

                        program.clear();
                        break;
                    case TokenType::RUN:

                        program.run();
                        break;

                    case TokenType::HELP:
                        std::cout << "A basic Interpreter\n";
                        break;
                    default:
                        Aline.getStatement()->execute(program.get_vars(), program);
                        delete Aline.fetchStatement();
                }
            }
        }
        catch (const BasicError &e)
        {
            std::cout << e.message() << "\n";
        }
    }
    return 0;
}
