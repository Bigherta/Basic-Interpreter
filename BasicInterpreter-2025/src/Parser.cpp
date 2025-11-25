#include "../include/Parser.hpp"

#include <memory>
#include <stdexcept>
#include <string>
#include "../include/Expression.hpp"
#include "../include/Statement.hpp"
#include "../include/utils/Error.hpp"

ParsedLine::ParsedLine() { statement_ = nullptr; } // 默认构造函数


void ParsedLine::setLine(int line) { line_number_.emplace(line); } // 将line作为当前行

std::optional<int> ParsedLine::getLine() { return line_number_; } // 获取当前行号

void ParsedLine::setStatement(std::shared_ptr<Statement> stmt) { statement_ = stmt; } // 将stmt作为当前statement

std::shared_ptr<Statement> ParsedLine::getStatement() const { return statement_; } // 获取当前statement的指针

std::shared_ptr<Statement> ParsedLine::fetchStatement()
{
    std::shared_ptr<Statement> temp = statement_;
    statement_ = nullptr;
    return temp;
} // 提取当前指针后将当前指针设置为空

ParsedLine Parser::parseLine(TokenStream &tokens, const std::string &originLine) const
{
    ParsedLine result;

    // 检查是否有行号
    const std::shared_ptr<Token> firstToken = tokens.peek();
    if (firstToken && firstToken->type == TokenType::NUMBER)
    {
        // 解析行号
        result.setLine(parseLiteral(firstToken));
        tokens.get(); // 消费行号token

        // 如果只有行号，表示删除该行
        if (tokens.empty())
        {
            return result;
        }
    }

    // 解析语句
    result.setStatement(parseStatement(tokens, originLine));

    return result;
}

std::shared_ptr<Statement> Parser::parseStatement(TokenStream &tokens, const std::string &originLine) const
{
    if (tokens.empty())
    {
        throw BasicError("SYNTAX ERROR");
    }

    const std::shared_ptr<Token> token = tokens.get();
    if (!token)
    {
        throw BasicError("SYNTAX ERROR");
    }

    switch (token->type)
    {
        case TokenType::LET:
            return parseLet(tokens, originLine);
        case TokenType::PRINT:
            return parsePrint(tokens, originLine);
        case TokenType::INPUT:
            return parseInput(tokens, originLine);
        case TokenType::GOTO:
            return parseGoto(tokens, originLine);
        case TokenType::IF:
            return parseIf(tokens, originLine);
        case TokenType::REM:
            return parseRem(tokens, originLine);
        case TokenType::END:
            return parseEnd(tokens, originLine);
        case TokenType::INDENT:
            return parseIndent(tokens, originLine);
        case TokenType::DEDENT:
            return parseDedent(tokens, originLine);
        default:
            throw BasicError("SYNTAX ERROR");
    }
}

std::shared_ptr<Statement> Parser::parseLet(TokenStream &tokens, const std::string &originLine) const
{
    if (tokens.empty())
    {
        throw BasicError("SYNTAX ERROR");
    }

    const std::shared_ptr<Token> varToken = tokens.get(); // 提取命名标志
    if (!varToken || varToken->type != TokenType::IDENTIFIER)
    {
        throw BasicError("SYNTAX ERROR");
    } // 若不是命名标志，异常抛出

    std::string varName = varToken->text;

    if (tokens.empty() || tokens.get()->type != TokenType::EQUAL)
    {
        throw BasicError("SYNTAX ERROR");
    } // 若为空或不为赋值符号，异常抛出

    auto expr = parseExpression(tokens);

    std::shared_ptr<LetStatement> temp = std::make_shared<LetStatement>(LetStatement(originLine, expr));
    return temp;
} // finished

std::shared_ptr<Statement> Parser::parsePrint(TokenStream &tokens, const std::string &originLine) const
{
    auto expr = parseExpression(tokens); // 解析表达式

    std::shared_ptr<PrintStatement> temp = std::make_shared<PrintStatement>(PrintStatement(originLine, expr));
    return temp;
} // finished

std::shared_ptr<Statement> Parser::parseInput(TokenStream &tokens, const std::string &originLine) const
{
    if (tokens.empty())
    {
        throw BasicError("SYNTAX ERROR");
    }

    const std::shared_ptr<Token> varToken = tokens.get();
    if (!varToken || varToken->type != TokenType::IDENTIFIER)
    {
        throw BasicError("SYNTAX ERROR");
    }

    std::string varName = varToken->text;

    std::shared_ptr<InputStatement> temp = std::make_shared<InputStatement>(InputStatement(originLine, varName));
    return temp;
} // finished

std::shared_ptr<Statement> Parser::parseGoto(TokenStream &tokens, const std::string &originLine) const
{
    if (tokens.empty())
    {
        throw BasicError("SYNTAX ERROR");
    }

    const std::shared_ptr<Token> lineToken = tokens.get();
    if (!lineToken || lineToken->type != TokenType::NUMBER)
    {
        throw BasicError("SYNTAX ERROR");
    }

    int targetLine = parseLiteral(lineToken);

    std::shared_ptr<GOTOstatement> temp = std::make_shared<GOTOstatement>(GOTOstatement(originLine, targetLine));

    return temp;
} // finished

std::shared_ptr<Statement> Parser::parseIf(TokenStream &tokens, const std::string &originLine) const
{
    // 解析左表达式
    auto leftExpr = parseExpression(tokens);

    if (tokens.empty())
    {
        throw BasicError("SYNTAX ERROR");
    }

    // 解析比较操作符
    const std::shared_ptr<Token> opToken = tokens.get();
    char op;
    switch (opToken->type)
    {
        case TokenType::EQUAL:
            op = '=';
            break;
        case TokenType::GREATER:
            op = '>';
            break;
        case TokenType::LESS:
            op = '<';
            break;
        default:
            throw BasicError("SYNTAX ERROR");
    }

    // 解析右表达式
    auto rightExpr = parseExpression(tokens);

    // 检查THEN关键字
    if (tokens.empty() || tokens.get()->type != TokenType::THEN)
    {
        throw BasicError("SYNTAX ERROR");
    }

    // 解析目标行号
    if (tokens.empty())
    {
        throw BasicError("SYNTAX ERROR");
    }

    const std::shared_ptr<Token> lineToken = tokens.get();
    if (!lineToken || lineToken->type != TokenType::NUMBER)
    {
        throw BasicError("SYNTAX ERROR");
    }

    int targetLine = parseLiteral(lineToken); // GOTO的目标行


    std::shared_ptr<IfStatement> temp =
            std::make_shared<IfStatement>(IfStatement(originLine, targetLine, leftExpr, rightExpr, op));
    return temp;
}

std::shared_ptr<Statement> Parser::parseRem(TokenStream &tokens, const std::string &originLine) const
{
    const std::shared_ptr<Token> remInfo = tokens.get();
    if (!remInfo || remInfo->type != TokenType::REMINFO)
    {
        throw BasicError("SYNTAX ERROR");
    }

    std::shared_ptr<RemStatement> temp = std::make_shared<RemStatement>(RemStatement(originLine));
    return temp;
}

std::shared_ptr<Statement> Parser::parseEnd(TokenStream &tokens, const std::string &originLine) const
{

    std::shared_ptr<EndStatement> temp = std::make_shared<EndStatement>(EndStatement(originLine));
    return temp;
}

std::shared_ptr<Statement> Parser::parseIndent(TokenStream &tokens, const std::string &originLine) const
{

    std::shared_ptr<IndentStatement> temp = std::make_shared<IndentStatement>(IndentStatement(originLine));
    return temp;
}

std::shared_ptr<Statement> Parser::parseDedent(TokenStream &tokens, const std::string &originLine) const
{

    std::shared_ptr<DedentStatement> temp = std::make_shared<DedentStatement>(DedentStatement(originLine));
    return temp;
}

std::shared_ptr<Expression> Parser::parseExpression(TokenStream &tokens) const { return parseExpression(tokens, 0); }

std::shared_ptr<Expression> Parser::parseExpression(TokenStream &tokens, int precedence) const
{
    // 解析左操作数
    std::shared_ptr<Expression> left;

    if (tokens.empty())
    {
        throw BasicError("SYNTAX ERROR");
    } // 检查是否为空

    const std::shared_ptr<Token> token = tokens.get();
    if (!token)
    {
        throw BasicError("SYNTAX ERROR");
    } // 检查是否有效

    if (token->type == TokenType::NUMBER)
    {
        int value = parseLiteral(token);
        left = std::make_shared<ConstExpression>(ConstExpression(value));
    } // 将左操作数赋值为常量
    else if (token->type == TokenType::IDENTIFIER)
    {
        left = std::make_shared<VariableExpression>(VariableExpression(token->text));
    } // 将左操作数作为变量表达式并命名为token->text
    else if (token->type == TokenType::LEFT_PAREN)
    {
        ++leftParentCount; // 左括号数加一
        left = parseExpression(tokens, 0);
        // 递归解析剩下的表达式
        if (tokens.empty() || tokens.get()->type != TokenType::RIGHT_PAREN)
        {
            throw BasicError("MISMATCHED PARENTHESIS");
        }
        --leftParentCount;
    }
    else
    {
        throw BasicError("SYNTAX ERROR");
    }

    // 检查是否有运算符
    while (!tokens.empty())
    {
        const std::shared_ptr<Token> opToken = tokens.peek();
        if (!opToken)
        {
            break;
        }

        // 检查是否是右括号
        if (opToken->type == TokenType::RIGHT_PAREN)
        {
            if (leftParentCount == 0)
            {
                throw BasicError("MISMATCHED PARENTHESIS");
            }
            break;
        }

        // 检查是否是运算符
        int opPrecedence = getPrecedence(opToken->type);
        if (opPrecedence == -1 || opPrecedence < precedence)
        {
            break;
        }

        tokens.get(); // 消费运算符token

        char op;
        switch (opToken->type)
        {
            case TokenType::PLUS:
                op = '+';
                break;
            case TokenType::MINUS:
                op = '-';
                break;
            case TokenType::MUL:
                op = '*';
                break;
            case TokenType::DIV:
                op = '/';
                break;
            default:
                throw BasicError("SYNTAX ERROR");
        }

        // 解析右操作数，使用更高的优先级
        auto right = parseExpression(tokens, opPrecedence + 1);
        left = std::make_shared<CompoundExpression>(CompoundExpression(left, op, right));
    }

    return left;
}

int Parser::getPrecedence(TokenType op) const
{
    switch (op)
    {
        case TokenType::PLUS:
        case TokenType::MINUS:
            return 1;
        case TokenType::MUL:
        case TokenType::DIV:
            return 2;
        default:
            return -1;
    }
}

int Parser::parseLiteral(const std::shared_ptr<Token> token) const
{
    if (!token || token->type != TokenType::NUMBER)
    {
        throw BasicError("SYNTAX ERROR");
    }

    try
    {
        size_t pos;
        int value = std::stoi(token->text, &pos);

        // 检查是否整个字符串都被解析
        if (pos != token->text.length())
        {
            throw BasicError("INT LITERAL OVERFLOW");
        }

        return value;
    }
    catch (const std::out_of_range &)
    {
        throw BasicError("INT LITERAL OVERFLOW");
    }
    catch (const std::invalid_argument &)
    {
        throw BasicError("SYNTAX ERROR");
    }
}
