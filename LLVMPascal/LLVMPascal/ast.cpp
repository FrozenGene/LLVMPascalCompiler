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

    BlockAST::BlockAST(const TokenLocation& loc, const VecExprASTPtr& body)
        : ExprAST(loc), body_(body)
    {}

    ProgramAST::ProgramAST(const TokenLocation& loc, const std::string& programName)
        : ExprAST(loc), programName_(programName)
    {}

    IfStatementAST::IfStatementAST(const TokenLocation& loc, ExprASTPtr condition, ExprASTPtr thenPart, ExprASTPtr elsePart)
        : ExprAST(loc), condition_(std::move(condition)), thenPart_(std::move(thenPart)), elsePart_(std::move(elsePart))
    {}

    WhileStatementAST::WhileStatementAST(const TokenLocation& loc, ExprASTPtr condition, ExprASTPtr body)
        : ExprAST(loc), condition_(std::move(condition)), body_(std::move(body))
    {}

    ForStatementAST::ForStatementAST(const TokenLocation& loc, const std::string& controlVariable,
        ExprASTPtr startExpr, ExprASTPtr endExpr, bool downOrder, ExprASTPtr body)
        : ExprAST(loc), controlVariable_(controlVariable), startExpr_(std::move(startExpr)),
        endExpr_(std::move(endExpr)), downOrder_(downOrder), body_(std::move(body))
    {}

    RepeatStatementAST::RepeatStatementAST(const TokenLocation& loc, ExprASTPtr condition, BlockASTPtr body)
        : ExprAST(loc), condition_(std::move(condition)), body_(std::move(body))
    {}

    AssignStatementAST::AssignStatementAST(const TokenLocation& loc, ExprASTPtr lhs, ExprASTPtr rhs)
        : ExprAST(loc), lhs_(std::move(lhs)), rhs_(std::move(rhs))
    {}
}
