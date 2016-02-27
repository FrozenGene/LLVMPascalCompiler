/**********************************
* File:    error.cpp
*
* Author:  Wu Zhao
* 
* Email:   wuzhaozju@gmail.com
* 
* Date:    2014/08/03
*
* License: BSD
*********************************/
#include <iostream>
#include "error.h"
#include "parser.h"

namespace llvmpascal
{
    void errorToken(const std::string& msg)
    {
        std::cerr << "Token Error:" << msg << std::endl;
        Scanner::setErrorFlag(true);
    }

    void errorSyntax(const std::string& msg)
    {
        std::cerr << "Syntax Error: " << msg << std::endl;
        Parser::setErrorFlag(true);
    }
}