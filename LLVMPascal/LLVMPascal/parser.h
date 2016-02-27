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
    using VecExprASTPtr = std::vector <std::unique_ptr<ExprAST>>;
    using ExprASTPtr = std::unique_ptr <ExprAST>;
    using VarExprASTPtr = std::unique_ptr <VariableAST>;
    using BlockASTPtr = std::unique_ptr <BlockAST>;
    using PrototypeASTPtr = std::unique_ptr <PrototypeAST>;
    using FunctionASTPtr = std::unique_ptr <FunctionAST>;
    using VarDeclASTPtr = std::unique_ptr <VariableDeclarationAST>;
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
        // parseIdentifierExpr, parseParenExpr functions are just like 
        // llvm kaleidoscope tutorial 02
        // see the link: http://llvm.org/docs/tutorial/LangImpl2.html
        ExprASTPtr            parseExpression();
        ExprASTPtr            parsePrimary();
        ExprASTPtr            parseBinOpRHS(int precedence, ExprASTPtr lhs);
        ExprASTPtr            parseUnaryOp();
        ExprASTPtr            parseIdentifierExpr();
        ExprASTPtr            parseParenExpr();
        ExprASTPtr            parseSetExpr();

        // parse different type expressions.
        // they are also very like parseNumber function in the llvm tutorial,
        // but they are more complex
        ExprASTPtr            parseRealExpr(Token token);
        ExprASTPtr            parseIntegerExpr(Token token);
        ExprASTPtr            parseCharExpr(Token token);
        ExprASTPtr            parseStringExpr(Token token);

        // leave set, array, filed, pointer type expression to implementation.
        /* 
        ExprASTPtr            parseSetExpr();
        VarExprASTPtr         parseArrayExpr(VarExprASTPtr varExpr, Types::TypeDecl*& type);
        VarExprASTPtr         parseFieldExpr(VarExprASTPtr varExpr, Types::TypeDecl*& type);
        VarExprASTPtr         parsePointerExpr(VarExprASTPtr varExpr, Types::TypeDecl*& type);
        */
        
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

        ExprASTPtr            parseGotoStatement(){/* TODO: Maybe I will not implement it */}

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