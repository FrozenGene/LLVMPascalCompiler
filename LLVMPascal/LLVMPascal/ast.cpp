/**********************************
* File:    ast.cpp
*
* Author:  Wu Zhao
*
* Email:   wuzhaozju@gmail.com
*
* Date:    2015/06/15
*
* License: BSD
*********************************/
#include "ast.h"

namespace llvmpascal
{
    ExprAST::ExprAST(const TokenLocation& loc) 
        : loc_(loc)
    {}

    ProgramAST::ProgramAST(const TokenLocation& loc, const std::string& programName)
        : ExprAST(loc), programName_(programName)
    {}

    IfStatementAST::IfStatementAST(const TokenLocation& loc, ExprASTPtr condition, ExprASTPtr thenPart, ExprASTPtr elsePart)
        : ExprAST(loc), condition_(std::move(condition)), thenPart_(std::move(thenPart)), elsePart_(std::move(elsePart))
    {}
}