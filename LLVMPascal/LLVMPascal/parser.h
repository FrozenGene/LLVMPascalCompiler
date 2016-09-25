/**********************************
* File:    parser.h
*
* Author:  Wu Zhao
*
* Email:   wuzhaozju@gmail.com
*
* Date:    2015/06/15
*
* License: BSD
*********************************/

#ifndef PARSER_H_
#define PARSER_H_

#include <vector>
#include <memory>
#include "scanner.h"
#include "ast.h"
#include "constant.h"

namespace llvmpascal
{
    // others XXXPtr* such as ExprASTPtr) are located in the ast.h
    using ConstantDeclPtr = std::unique_ptr <Constant>;

    class Parser
    {
    public:
        explicit              Parser(Scanner& scanner);
        static bool           getErrorFlag();
        static void           setErrorFlag(bool flag);
        VecExprASTPtr&        parse();

    private:
        // parseExpression, parsePrimary, parseBinOpRHS and parseUnaryOp,
        // parseIdentifierExpression, parseParenExpression functions are just like 
        // llvm kaleidoscope tutorial 02
        // see the link: http://llvm.org/docs/tutorial/LangImpl2.html
        ExprASTPtr            parseExpression();
        ExprASTPtr            parsePrimary();
        ExprASTPtr            parseBinOpRHS(int precedence, ExprASTPtr lhs);
        ExprASTPtr            parseUnaryOp();
        ExprASTPtr            parseIdentifierExpression();
        ExprASTPtr            parseParenExpression();
        

        // parse different type expressions.
        // they are also very like parseNumber function in the llvm tutorial,
        // but they are more complex
        ExprASTPtr            parseRealExpression(Token token);
        ExprASTPtr            parseIntegerExpression(Token token);
        ExprASTPtr            parseCharExpression(Token token);
        ExprASTPtr            parseStringExpression(Token token);

        // set, array, filed, pointer type expression to implementation.
        ExprASTPtr            parseSetExpression();

        // see pascal standard 6.8
        
        // note: assignment statement implementation will be included parseStatement.
        ExprASTPtr            parseStatement(); 
        ExprASTPtr            parseIfStatement();
        ExprASTPtr            parseCaseStatement();
        ExprASTPtr            parseRepeatStatement();
        ExprASTPtr            parseWhileStatement();
        ExprASTPtr            parseForStatement();
        ExprASTPtr            parseWithStatement();
        ExprASTPtr            parseProgramStatement();

        ExprASTPtr            parseGotoStatement();// TODO: Maybe I will not implement it.

        BlockASTPtr           parseBlockStatement(); // begin...end
        ExprASTPtr            parseBlockOrStatement(); // if not begin

        // declaration / definition contains procedure and function.
        // see pascal standard 6.7 and 6.8
        // also see the link: http://pascal-programming.info/lesson7.php
        PrototypeASTPtr       parseFunctionDeclaration(bool isFunction = true);
        FunctionASTPtr        parseFunctionDefinition(int functionLevel);
        VarDeclASTPtr         parseVariableDeclaration();

        // I/O routines
        ExprASTPtr            parseRead();
        ExprASTPtr            parseWrite();

        // Type
        void                  parseTypeDefinition();
        void                  parseConstantDefinition();
        ConstantDeclPtr       parseConstantExpression();
        Token                 parseToken(const Token& token);
        // TODO:
        // ........... many types related ............

    // Helper Functions.
    private:
        bool                  expectToken(TokenValue value, const std::string& tokenName, bool advanceToNextToken);
        bool                  expectToken(TokenType type, const std::string& tokenTypeDescription, bool advanceToNextToken);
        bool                  validateToken(TokenValue value, bool advanceToNextToken);
        bool                  validateToken(TokenType type, bool advanceToNextToken);
        void                  errorReport(const std::string& msg);
    private:
        Scanner&              scanner_;
        VecExprASTPtr         ast_;
        static bool           errorFlag_;

    };

    inline bool Parser::getErrorFlag()
    {
        return errorFlag_;
    }

}

#endif // parser.h