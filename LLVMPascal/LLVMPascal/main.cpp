/**********************************
* File:    main.cpp
*
* Author:  Wu Zhao
*
* Email:   wuzhaozju@gmail.com
*
* Date:    2014/08/04
*
* License: BSD
*********************************/


#include <fstream>
#include <iostream>
#include <string>
#include "token.h"
#include "dictionary.h"
#include "scanner.h"

using namespace llvmpascal;

int main()
{
    Scanner scanner("scanner_test.pas");

    scanner.getNextToken();

    while (scanner.getToken().getTokenType() != TokenType::END_OF_FILE)
    {
        if (scanner.getErrorFlag())
        {
            return 1;
        }
        scanner.getToken().dump();
        scanner.getNextToken();
    }

    return 0;
}