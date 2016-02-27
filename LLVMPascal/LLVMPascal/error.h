/**********************************
* File:    error.h
*
* Author:  Wu Zhao
* 
* Email:   wuzhaozju@gmail.com
* 
* Date:    2014/08/03
*
* License: BSD
*********************************/

#ifndef ERROR_H_
#define ERROR_H_

#include <string>
#include <cassert>

namespace llvmpascal
{
    extern void errorToken(const std::string& msg);
    extern void errorSyntax(const std::string& msg);
}

#endif // error.h