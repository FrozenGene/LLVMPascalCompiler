/**********************************
* File:    ast.h
*
* Author:  Wu Zhao
* 
* Email:   wuzhaozju@gmail.com
* 
* Date:    2015/06/15
*
* License: BSD
*********************************/

#ifndef AST_H_
#define AST_H_

#include <string>
#include <vector>
#include <memory>
#include "token.h"

namespace llvmpascal
{
    // this is very like llvm kaleidoscope tutorial 02
    // in fact, the prototype is also from there, but it
    // will become different as the project become complex.
    // but now, it will be simple as the tutorial.
    // additionally, I just leave empty implementation when
    // I implement parser part.
    // see the link: http://llvm.org/docs/tutorial/LangImpl2.html
    
    // base class. I will consider to use llvm style rtti rather than C++ standard rtti
    // to intereact with LLVM API more smoothly. (MAYBE, not decided now.)
    // see the link: http://llvm.org/docs/HowToSetUpLLVMStyleRTTI.html

    class ExprAST;
    class VariableAST;
    class BlockAST;
    class PrototypeAST;
    class FunctionAST;
    class VariableDeclarationAST;

    using VecExprASTPtr = std::vector <std::unique_ptr<ExprAST>>;
    using ExprASTPtr = std::unique_ptr <ExprAST>;
    using VarExprASTPtr = std::unique_ptr <VariableAST>;
    using BlockASTPtr = std::unique_ptr <BlockAST>;
    using PrototypeASTPtr = std::unique_ptr <PrototypeAST>;
    using FunctionASTPtr = std::unique_ptr <FunctionAST>;
    using VarDeclASTPtr = std::unique_ptr <VariableDeclarationAST>;

    class ExprAST
    {
    public:
        ExprAST(const TokenLocation& loc);
        virtual       ~ExprAST() = default;

    private:
        TokenLocation loc_;

    };

    class ProgramAST : public ExprAST
    {
    public:
        explicit      ProgramAST(const TokenLocation& loc, const std::string& programName);

    private:
        std::string   programName_;
    };

    class VariableAST : public ExprAST
    {

    };

    class BlockAST : public ExprAST
    {
    public:
        BlockAST(const TokenLocation& loc, const VecExprASTPtr& body);

    private:
        const VecExprASTPtr& body_;
    };

    class FunctionAST : public ExprAST
    {

    };

    class PrototypeAST : public ExprAST
    {

    };

    class VariableDeclarationAST : public ExprAST
    {

    };

    class IfStatementAST : public ExprAST
    {
    public:
        IfStatementAST(const TokenLocation& loc, ExprASTPtr condition, ExprASTPtr thenPart, ExprASTPtr elsePart);

    private:
        ExprASTPtr    condition_;
        ExprASTPtr    thenPart_;
        ExprASTPtr    elsePart_;
    };

    class WhileStatementAST : public ExprAST
    {
    public:
        WhileStatementAST(const TokenLocation& loc, ExprASTPtr condition, ExprASTPtr body);

    private:
        ExprASTPtr   condition_;
        ExprASTPtr   body_;

    };

    class ForStatementAST : public ExprAST
    {
    public:
        ForStatementAST(const TokenLocation& loc, const std::string& controlVariable, ExprASTPtr startExpr, ExprASTPtr endExpr,
            bool downOrder, ExprASTPtr body);

    private:
        std::string controlVariable_;
        ExprASTPtr  startExpr_;
        ExprASTPtr  endExpr_;
        bool        downOrder_;
        ExprASTPtr  body_;

    };

    class RepeatStatementAST : public ExprAST
    {
    public:
        RepeatStatementAST(const TokenLocation& loc, ExprASTPtr condition, BlockASTPtr body);

    private:
        ExprASTPtr condition_;
        BlockASTPtr body_;

    };



}

#endif // ast.h