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
    // to interect LLVM API more smoothly. (MAYBE, not decided now.)
    // see the link: http://llvm.org/docs/HowToSetUpLLVMStyleRTTI.html
    class ExprAST
    {
    public:
        virtual     ~ExprAST() = default;
    };

    class ProgramAST : public ExprAST
    {
    public:
        explicit    ProgramAST(const std::string& programName);

    private:
        std::string programName_;
    };

    class VariableAST : public ExprAST
    {

    };

    class BlockAST : public ExprAST
    {

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

}

#endif // ast.h