/**********************************
* File:    parser.cpp
*
* Author:  Wu Zhao
*
* Email:   wuzhaozju@gmail.com
*
* Date:    2015/09/07
*
* License: BSD
*********************************/
#include "parser.h"
#include "error.h"
#include "constant.h"

namespace llvmpascal
{
    bool Parser::errorFlag_ = false;

    Parser::Parser(Scanner& scanner):scanner_(scanner)
    {
        // Eat the first token.
        scanner_.getNextToken();
    }


    // Pascal Program Basic Structure.
    // see: http://pirate.shu.edu/~wachsmut/Teaching/CSAS1111/Notes-Pascal/pascal1.html

    /*

    PROGRAM ProgramName;

    VAR
    VariableName : VariableType;
    VariableName : VariableType;
    (*...*)

    PROCEDURE ProcedureName;
        variables here if necessary
    BEGIN
        Some Code;
    END;

    FUNCTION FunctionName(variableList): VariableType;
        variables here if necessary
    BEGIN
        Some Code if necessary;
        FunctionName := some expression
        More Code if necessary;
    END;

    (*... more functions and procedures if necessary ...*)

    BEGIN
        the main program block. It should be small and all
        work should be delegated to the procedures and
        functions.
        It often consists of a WHILE loop that calls in
        turn procedures and functions in the appropriate
        order.
    END.


    Meanwhile, according to Pascal standard 6.2.1

    block = label-declaration-part constant-definition-part type-definition-part
            variable-declaration-part procedure-and-function-declaration-part
            statement-part

    label-declaration-part = [ 'label' label { ',' label } ';' ]
    constant-definition-part = [ 'const' constant-definition ' ;' { constant-definition ' ;' }]
    type-definition-part = [ 'type' type-definition ' ;' { type-definition ';' } ]
    variable-declaration-part = [ 'var' variable-declaration ';' { variable-declaration ';' }]
    procedure-and-function-declaration-part = { ( procedure-declaration | function-declaration ';') }

    So, we should contain these parts into the basic program structure we mentioned just now omitted,
    such as constant definition part, type definition part and so on.

    */

    VecExprASTPtr& Parser::parse()
    {
        if (!parseProgramStatement())
        {
            return ast_;
        }

        if (scanner_.getToken().getTokenType() == TokenType::END_OF_FILE)
        {
            errorReport("Unexpected end of file.");
            ast_.clear();
            return ast_;
        }

        // TODO: input, output variable declaration. They will be text type
        // according to Pascal standard (but I have not added type system now, just
        // leave here).
        //
        // The original quote:
        // The occurrence of the required identifier input or output as a
        // program-parameter shall constitute its defining-point for the
        // region that is the program-block as a variable-identifier of
        // the required type denoted by the required type-identifier text
        //

        for (;;)
        {
            std::unique_ptr<ExprAST> currentASTPtr = nullptr;
            switch (scanner_.getToken().getTokenValue())
            {
                case TokenValue::BEGIN:
                {
                    BlockASTPtr programBoby = parseBlockStatement();

                    // Leave one function called __PASCAL_MAIN__ implementation.
                    // It will be called by C.

                    if (!expectToken(TokenValue::PERIOD, ".", true))
                    {
                        ast_.clear();
                        return ast_;
                    }

                    return ast_;
                }

                case TokenValue::SEMICOLON:
                {
                    scanner_.getNextToken();
                    break;
                }

                case TokenValue::FUNCTION:
                case TokenValue::PROCEDURE:
                {
                    currentASTPtr = parseFunctionDefinition(0);
                    break;
                }

                case TokenValue::VAR:
                {
                    currentASTPtr = parseVariableDeclaration();
                    break;
                }

                // "TYPE" and "CONST" does not have AST. Just continue.
                case TokenValue::TYPE:
                {
                    parseTypeDefinition();
                    continue;
                }

                case TokenValue::CONST:
                {
                    parseConstantDefinition();
                    continue;
                }

                default:
                {
                    // It is very like LLVM Kaleidoscope tutorial "ParseExpression" :-)
                    currentASTPtr = parseExpression();
                    break;
                }
            }

            if (currentASTPtr != nullptr)
            {
                ast_.push_back(std::move(currentASTPtr));
            }
        }
    }


    // program = program-heading  ';' program-block '.' .
    // program-heading = 'program' identifier [ '(' program-parameter-list ')' ]
    // program-parameter-list = identifier-list .
    // program-block = block .

    // see pascal standard 6.10

    // diagram show:
    /*
       PROGRAM -> IDENTIFIER -> [ '(' -> IDENTIFIER [, IDENTIFIER ... ] -> ')' ] -> ';'

    */

    // Example:
    // PROGRAM helloworld;
    // PROGRAM helloworld(input, output);

    ExprASTPtr Parser::parseProgramStatement()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if (!expectToken(TokenValue::PROGRAM, "program", true))
        {
            return nullptr;
        }

        if (!expectToken(TokenType::IDENTIFIER, "identifier", false))
        {
            return nullptr;
        }

        std::string programName = scanner_.getToken().getTokenName();

        scanner_.getNextToken();

        // if there is '(', eat it and go on.
        // Otherwise, just stay here and wait to parser's next command :-)
        // In fact, if we want to do better, we can make the second parameter of
        // validateToken is false, then use expectToken function to give user better
        // error message about LEFT_PAREN / RIGHT PAREN, if you like it, modify and try!
        if (validateToken(TokenValue::LEFT_PAREN, true))
        {
            do
            {
                if (!expectToken(TokenType::IDENTIFIER, "identifier", true))
                {
                    return nullptr;
                }

                if (!validateToken(TokenValue::RIGHT_PAREN, false))
                {
                    if (!expectToken(TokenValue::COMMA, ",", true))
                    {
                        return nullptr;
                    }
                }

            } while (!validateToken(TokenValue::RIGHT_PAREN, true));

        }

        if (!expectToken(TokenValue::SEMICOLON, ";", true))
        {
            return nullptr;
        }

        return std::make_unique<ProgramAST>(loc, programName);
    }

    BlockASTPtr Parser::parseBlockStatement()
    {
        auto loc = scanner_.getToken().getTokenLocation();

        if(!expectToken(TokenValue::BEGIN , "begin", true))
        {
            return nullptr;
        }

        VecExprASTPtr stmts;

        while(!validateToken(TokenValue::END, false))
        {
            if(auto stmt = parseStatement())
            {
                stmts.push_back(std::move(stmt));

                if (!expectToken(TokenValue::SEMICOLON, ";", true) || !expectToken(TokenValue::END, "end", true))
                {
                    return nullptr;
                }
            }
            else
            {
                return nullptr;
            }
        }

        if(!expectToken(TokenValue::END, "end", true))
        {
            return nullptr;
        }

        return std::make_unique<BlockAST>(loc, stmts);
    }

    FunctionASTPtr Parser::parseFunctionDefinition(int functionLevel)
    {
        // TODO
        assert(0 && "I have not implemented parseFunctionDefinition.");
        return nullptr;
    }

    VarDeclASTPtr Parser::parseVariableDeclaration()
    {
        // TODO
        assert(0 && "I have not implemented parseVariableDeclaration.");
        return nullptr;
    }

    void Parser::parseTypeDefinition()
    {
        // TODO
        assert(0 && "I have not implemented parseTypeDefinition.");
        return;
    }

    // constant-definition-part = 'const' constant-definition ' ;' { constant-definition ' ;' }
    // constant-definition = identifier '=' constant
    // constant = [sign](unsigned - number | constant - identifier)
    //          | character-string

    /*
       [Example]

       const
           name = 'Blue';
           year = 2016;
           pi = 3.14159;

       [/Example]

    */
    void Parser::parseConstantDefinition()
    {
        if (!expectToken(TokenValue::CONST, "const", true))
        {
            return;
        }

        do
        {
            if (!expectToken(TokenType::IDENTIFIER, "identifier", false))
            {
                return;
            }

            std::string constIdentifierName = scanner_.getToken().getTokenName();

            scanner_.getNextToken();

            if (!expectToken(TokenValue::EQUAL, "=", true))
            {
                return;
            }

            ConstantDeclPtr constValue = parseConstantExpression();

            // DEBUG: constant value output
            constValue->dump();

            // TODO: add const to symbol table
            // addToSymbolTable(constIdentifierName, constVale);

            if (!expectToken(TokenValue::SEMICOLON, ";", true))
            {
                return;
            }


        } while (scanner_.getToken().getTokenType() == TokenType::IDENTIFIER);
    }

    // constant = [sign](unsigned - number | constant - identifier) | character-string
    // sign = '+' | '-'
    ConstantDeclPtr Parser::parseConstantExpression()
    {
        ConstantDeclPtr constDeclPtr = nullptr;
        TokenLocation tokenLocation = scanner_.getToken().getTokenLocation();

        // '+' / '-'
        // if it is '+', the value will be 1
        // if it is '-', the value will be -1
        int numberSign = 1;

        // for char type check
        bool hasNumberSign = false;

        // some types can have logic operator NOT
        bool notOpFlag = false;

        do
        {
            // '('
            // recursive parse and hanle next token.
            if (validateToken(TokenValue::LEFT_PAREN, true))
            {
                constDeclPtr = parseConstantExpression();

                if (!expectToken(TokenValue::RIGHT_PAREN, "(", false))
                {
                    return nullptr;
                }
            }

            // whether we have sign or unary prefix before token.
            switch (scanner_.getToken().getTokenValue())
            {
                case TokenValue::MINUS:
                {
                    hasNumberSign = true;
                    numberSign = -1;
                    scanner_.getNextToken();
                    break;
                }

                case TokenValue::PLUS:
                {
                    hasNumberSign = true;
                    numberSign = 1;
                    scanner_.getNextToken();
                    break;
                }

                case TokenValue::NOT:
                {
                    notOpFlag = true;
                    scanner_.getNextToken();
                    break;
                }

                default:
                {
                    hasNumberSign = false;
                    notOpFlag = false;
                    break;
                }
            }

            // now, we compute the constant expression
            switch (scanner_.getToken().getTokenType())
            {
                case TokenType::INTEGER:
                {
                    long integerValue = scanner_.getToken().getIntValue();

                    if (notOpFlag)
                    {
                        integerValue = ~integerValue;
                    }

                    constDeclPtr = std::make_unique<IntegerConstant>(integerValue * numberSign, tokenLocation);
                    break;
                }

                case TokenType::REAL:
                {
                    if (notOpFlag)
                    {
                        errorReport("'NOT' can not be used in the real constants");
                        return nullptr;
                    }

                    double realValue = scanner_.getToken().getRealValue();
                    constDeclPtr = std::make_unique<RealConstant>(realValue * numberSign, tokenLocation);
                    break;
                }

                case TokenType::CHAR:
                {
                    if (hasNumberSign)
                    {
                        errorReport("'+' or '-'  can not be uses in the char constants");
                        return nullptr;
                    }

                    long charValue = scanner_.getToken().getIntValue();

                    if (notOpFlag)
                    {
                        charValue = ~charValue;
                    }

                    constDeclPtr = std::make_unique<CharConstant>(static_cast<char>(charValue), tokenLocation);
                    break;
                 }

                case TokenType::STRING_LITERAL:
                {
                    if (hasNumberSign)
                    {
                        errorReport("'+' or '-'  can not be uses in the string literial");
                        return nullptr;
                    }

                    constDeclPtr = std::make_unique<StringConstant>(scanner_.getToken().getStringValue(), tokenLocation);
                    break;
                }

                case TokenType::IDENTIFIER:
                {
                    assert(0 && "Sorry, doesn't support const identifier now, I know it is valid, but I will implement it later.");
                    break;
                }

                default:
                {
                    assert(0 && "Should not reach here. If reach here, I will analysis the reason.");
                    break;
                }
            }

            // eat constant value token. Keep mind the proceduce.
            scanner_.getNextToken();
            if (!validateToken(TokenValue::SEMICOLON, false) &&
                !validateToken(TokenValue::RIGHT_PAREN, false))
            {
                assert(0 && "Does not support now. Should parse right op");
                // TODO:
                // constDeclPtr = parseConstRhs(precedence, constDeclPtr);
            }


        } while (!validateToken(TokenValue::SEMICOLON, false) &&
                 !validateToken(TokenValue::RIGHT_PAREN, false));

        return constDeclPtr;
    }

    ExprASTPtr Parser::parseExpression()
    {
        auto lhs = parsePrimary();

        if (lhs == nullptr)
        {
            return nullptr;
        }

        return parseBinOpRHS(0, std::move(lhs));
    }

    ExprASTPtr Parser::parsePrimary()
    {
        // if the token is const identifier, we will translate it to real value.
        Token token = parseToken(scanner_.getToken());

        // if token is constant value / identifier.
        // TODO:
        //
        // NOTE:
        // getTokenType???
        // MAYBE we can consider tokentype and then nest tokenvalue???
        // because I want only one entrance.

        // every token has one token type whether it is keywords or constant value
        switch (token.getTokenType())
        {
            // TODO:
            // CHAR / INTEGER / REAL ... constant

            // if token is keywords, if / while and so on
            case TokenType::KEYWORDS:
            {
                switch (token.getTokenValue())
                {
                    case TokenValue::IF:
                        return parseIfStatement();

                    case TokenValue::FOR:
                        return parseForStatement();

                    case TokenValue::WHILE:
                        return parseWhileStatement();

                    case TokenValue::REPEAT:
                        return parseRepeatStatement();

                    case TokenValue::CASE:
                        return parseCaseStatement();
                    // TODO:
                    // many others...
                    default:
                        assert(0 && "I have not completed parsePrimary part");
                }
            }

            case TokenType::DELIMITER:
            {
                switch (token.getTokenValue())
                {
                    case TokenValue::LEFT_PAREN:
                        return parseParenExpression();

                    case TokenValue::LEFT_SQUARE:
                        return parseSetExpression();

                    default:
                        assert(0 && "should not reach here, unexpected delimiter.");
                }
            }

            default:
            {
                // TODO:
                assert(0 && "Sorry, I have not completed parser work, I will do it soon.");
            }

        }

        return nullptr;

    }

    Token Parser::parseToken(const Token& token)
    {
        if (token.getTokenType() == TokenType::IDENTIFIER)
        {
            // TODO:
            // if this is a constant identifier, we will translate it to get its value.
            // i.e. call XXXConstant::getValue() function.
        }

        return token;
    }

    ExprASTPtr Parser::parseBinOpRHS(int precedence, ExprASTPtr lhs)
    {
        // TODO:
        assert(0 && "I have not implemented parseBinOpRHS.");
        return nullptr;
    }

    /*
    see pascal standard 6 .8.3 .4
    If-statements

    if-statement = 'if' Boolean-expression 'then' statement [ else-part ]
    else-part = 'else' statement

    [Example]

        if x < 1 .5 then z := x + y else z := 1 .5

        if p1 <> nil then p1 := p1

        if j = 0 then
            if i = 0 then writeln( ' indefinite ' )
            else writeln( ' infinite ' )
        else writeln( i / j )

    [/Example]
    */

    ExprASTPtr Parser::parseIfStatement()
    {
        // if we do it very well, we can get token if location,
        // token then location, token else token location like Clang.
        // but here we just get token if location for simplicity.
        // so, if you want to do better, just like Clang implementation.
        // I think it should not be difficult.
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        // current token is keyword if.
        if (!expectToken(TokenValue::IF, "if", true))
        {
            return nullptr;
        }

        auto condition = parseExpression();

        if (!condition)
        {
            errorReport("if condition is not valid.");
            return nullptr;
        }

        if (!expectToken(TokenValue::THEN, "then", true))
        {
            return nullptr;
        }

        ExprASTPtr thenPart = parseBlockOrStatement();

        if (!thenPart)
        {
            errorReport("then statement is not valid.");
            return nullptr;
        }

        ExprASTPtr elsePart = nullptr;

        // if we have token else.
        if (validateToken(TokenValue::ELSE, true))
        {
            elsePart = parseBlockOrStatement();
            if (!elsePart)
            {
                errorReport("else statement is not valid.");
                return nullptr;
            }
        }

        return std::make_unique<IfStatementAST>(loc, std::move(condition), std::move(thenPart), std::move(elsePart));
    }

    // 6.8.3.5 Case-statements
    // case-statement = 'case' case-index 'of' case-list-element
    //                  {' ;' case-list-element} [';'] 'end'
    //case-list-element = case-constant-list ':' statement
    //case-index = expression

    // For example
    /*

    [Example]

    case operator of
         plus : x := x + y;
         minus : x := x - y;
         times : x := x * y
    end

    [/Example]

    */
    ExprASTPtr Parser::parseCaseStatement()
    {
        // TODO:
        assert(0 && "I have not implemented parseCaseStatement.");
        return nullptr;
    }

    // 6.8.3.9 For-statements
    // for-statement = 'for' control-variable ':=' initial-value ( 'to' | 'downto' )
    //                  final-value 'do' statement
    //
    // control-variable = entire-variable
    // initial-value = expression
    // fi nal-value = expression

    // for v := e1 to | downto e2 do body
    ExprASTPtr Parser::parseForStatement()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if(!expectToken(TokenValue::FOR, "for", true))
        {
            return nullptr;
        }

        if(!validateToken(TokenType::IDENTIFIER, false))
        {
            errorReport("For statement control varible expected identifier type but find " + scanner_.getToken().toString());
            return nullptr;
        }

        auto controlVariable = scanner_.getToken().getIdentifierName();

        if(!expectToken(TokenValue::ASSIGN, ":=", true))
        {
            return nullptr;
        }

        auto startExpr = parseExpression();

        if(!startExpr)
        {
            return nullptr;
        }

        bool downOrder = false;

        if (validateToken(TokenValue::TO, false) ||
            validateToken(TokenValue::DOWNTO, false))
        {
            if (scanner_.getToken().getTokenValue() == TokenValue::DOWNTO)
            {
                downOrder = true;
            }
            scanner_.getNextToken();
        }
        else
        {
            errorReport("Expected to / downto keyword, but find " + scanner_.getToken().toString());
            return nullptr;
        }

        auto endExpr = parseExpression();

        if(!endExpr)
        {
            return nullptr;
        }

        if(!expectToken(TokenValue::DO, "do", true))
        {
            return nullptr;
        }

        auto body = parseBlockOrStatement();

        if(!body)
        {
            return nullptr;
        }

        return std::make_unique<ForStatementAST>(loc, controlVariable,
            std::move(startExpr), std::move(endExpr), downOrder, std::move(body));

    }

    // 6.8.3.8: While-statements
    // while-statement = 'while' Boolean-expression 'do' statement
    ExprASTPtr Parser::parseWhileStatement()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if(!expectToken(TokenValue::WHILE, "while", true))
        {
            return nullptr;
        }

        auto condition = parseExpression();

        if(!condition)
        {
            return nullptr;
        }

        if(!expectToken(TokenValue::DO, "do", true))
        {
            return nullptr;
        }

        auto body = parseBlockOrStatement();

        if(!body)
        {
            return nullptr;
        }

        return std::make_unique<WhileStatementAST>(loc, std::move(condition), std::move(body));
    }

    // 6.8.3.7: Repeate-statements
    // repeat-statement = 'repeat' statement-sequence 'until' Boolean-expression
    /*
    [Example]

      repeat
         stmt1;
         stmt2;
         stmt3
      until Boolean-expression

     [/Example]
    */
    // My implementation thought is push these statements(stmt1, stmt2, stmt3...) as
    // one block scope statement(i.e. begin stm1, stmt2, stm3... end).
    ExprASTPtr Parser::parseRepeatStatement()
    {
        TokenLocation loc = scanner_.getToken().getTokenLocation();

        if(!expectToken(TokenValue::REPEAT, "repeat", true))
        {
            return nullptr;
        }

        TokenLocation nestedLoc = scanner_.getToken().getTokenLocation();
        VecExprASTPtr nestedStmts;
        while(!validateToken(TokenValue::UNTIL, true))
        {
            auto stmt = parseBlockOrStatement();

            if(!stmt)
            {
                return nullptr;
            }

            nestedStmts.push_back(std::move(stmt));

            // if current token is not keyword 'until', it should be semicolon.
            // because the last stmt could have not semicolon(also can have), but
            // others must have.
            // if current token is keyword 'until', forward it(not eat it) to
            // while condition and break current loop.
            if(!validateToken(TokenValue::UNTIL, false))
            {
                if (!expectToken(TokenValue::SEMICOLON, ";", true))
                {
                    return nullptr;
                }
            }
        }

        auto condition = parseExpression();

        if(!condition)
        {
            return nullptr;
        }

        return std::make_unique<RepeatStatementAST>(loc, std::move(condition), std::make_unique<BlockAST>(nestedLoc, nestedStmts));
    }


    ExprASTPtr Parser::parseParenExpression()
    {
        // TODO:
        assert(0 && "I have not implemented parseParenExpression.");
        return nullptr;
    }

    ExprASTPtr Parser::parseSetExpression()
    {
        // TODO:
        assert(0 && "I have not implemented parseSetExpression.");
        return nullptr;
    }

    ExprASTPtr Parser::parseStatement()
    {
        if(auto expr = parsePrimary())
        {
            if(validateToken(TokenValue::ASSIGN, true))
            {
                auto loc = scanner_.getToken().getTokenLocation();
                auto rhs = parseExpression();

                if(!rhs)
                {
                    errorReport("Invalid syntax");
                    return nullptr;
                }

                // move assignment operator
                // transfer the ownship from the rhs to lhs. And as if call lhs.reset(rhs.release())
                // so the lhs of resource object is destroyed rightly.
                expr = std::make_unique<AssignStatementAST>(loc, std::move(expr), std::move(rhs));
            }

            return expr;
        }

        errorReport("Invalid syntax");
        return nullptr;
    }

    ExprASTPtr Parser::parseBlockOrStatement()
    {
        switch(scanner_.getToken().getTokenValue())
        {
        case TokenValue::BEGIN:
            return parseBlockStatement();
        case TokenValue::SEMICOLON:
            return std::make_unique<BlockAST>(scanner_.getToken().getTokenLocation(), VecExprASTPtr{});
        default:
            return parseStatement();
        }
    }

    // Helper Functions.

    bool Parser::expectToken(TokenValue value, const std::string& tokenName, bool advanceToNextToken)
    {
        if (scanner_.getToken().getTokenValue() != value)
        {
            errorReport("Expected ' " + tokenName + " ', but find " + scanner_.getToken().getTokenName());
            return false;
        }

        if (advanceToNextToken)
        {
            scanner_.getNextToken();
        }

        return true;
    }

    bool Parser::expectToken(TokenType type, const std::string& tokenTypeDescription, bool advanceToNextToken)
    {
        if (scanner_.getToken().getTokenType() != type)
        {
            errorReport("Expected ' " + tokenTypeDescription + " ', but find " + scanner_.getToken().tokenTypeDescription() + " " + scanner_.getToken().getTokenName());
            return false;
        }

        if (advanceToNextToken)
        {
            scanner_.getNextToken();
        }

        return true;
    }

    void Parser::setErrorFlag(bool flag)
    {
        errorFlag_ = flag;
    }

    // validate the token.
    // if validatation is true, return true. Otherwise return false.
    // meanwhile, if validation is true and need to advance to next token,
    // we will call getNextToken function.
    //
    // Note:
    // after call valadateToken, we always need to do things to handle different conditions,
    // i.e. if it is true, what will we do. if it is false. what will we do.

    bool Parser::validateToken(TokenValue value, bool advanceToNextToken)
    {
        if (scanner_.getToken().getTokenValue() != value)
        {
            return false;
        }

        if (advanceToNextToken)
        {
            scanner_.getNextToken();
        }

        return true;
    }

    bool Parser::validateToken(TokenType type, bool advanceToNextToken)
    {
        if (scanner_.getToken().getTokenType() != type)
        {
            return false;
        }

        if (advanceToNextToken)
        {
            scanner_.getNextToken();
        }

        return true;
    }

    void Parser::errorReport(const std::string& msg)
    {
        errorSyntax(scanner_.getToken().getTokenLocation().toString() + msg);
    }
}
