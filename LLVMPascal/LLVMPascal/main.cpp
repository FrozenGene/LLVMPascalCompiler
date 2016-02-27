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
#include "parser.h"
using namespace llvmpascal;

int main()
{
    Scanner scanner("program_test.pas");
    //Scanner scanner("scanner_test.pas");

    //scanner.getNextToken();

    //while (scanner.getToken().getTokenType() != TokenType::END_OF_FILE)
    //{
    //    scanner.getToken().dump();
    //    scanner.getNextToken();
    //}
    Parser parser(scanner);
    parser.parse();
    

    return 0;
}