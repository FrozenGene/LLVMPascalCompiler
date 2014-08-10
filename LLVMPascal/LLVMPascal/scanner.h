/**********************************
* File:    scanner.h
*
* Author:  Wu Zhao
*
* Email:   wuzhaozju@gmail.com
*
* Date:    2014/07/30
*
* License: BSD
*********************************/

#ifndef SCANNER_H_
#define SCANNER_H_

#include <fstream>
#include "token.h"
#include "dictionary.h"

namespace llvmpascal
{

    class Scanner
    {
      public:
        explicit        Scanner(const std::string& srcFileName);
        Token           getToken() const;
        Token           getNextToken();
        bool            getErrorFlag() const;

      private:
        void            getNextChar();
        char            peekChar();
        void            addToBuffer(char c);
        void            reduceBuffer();

        void            makeToken(TokenType tt, TokenValue tv,
                                  const TokenLocation& loc, std::string name, int symbolPrecedence);

        void            makeToken(TokenType tt, TokenValue tv,
                                  const TokenLocation& loc, long intValue, std::string name);

        void            makeToken(TokenType tt, TokenValue tv,
                                  const TokenLocation& loc, double realValue, std::string name);

        void            handleEOFState();
        void            handleIdentifierState();
        void            handleNumberState();
        void            handleStringState();
        void            handleOperationState();
        void            preprocess();
        void            handleLineComment();
        void            handleBlockComment();
        TokenLocation   getTokenLocation() const;

        void            handleDigit();
        void            handleXDigit();
        void            handleFraction();
        void            handleExponent();

      public:
        enum class State
        {
            NONE,
            END_OF_FILE,
            IDENTIFIER,
            NUMBER,
            STRING,
            OPERATION
        };

      private:
        std::string         fileName_;
        std::ifstream       input_;
        long                line_;
        long                column_;
        TokenLocation       loc_;
        char                currentChar_;
        bool                errorFlag_;
        State               state_;
        Token               token_;
        Dictionary          dictionary_;
        std::string         buffer_;

    };

    inline Token Scanner::getToken() const
    {
        return token_;
    }

    inline bool Scanner::getErrorFlag() const
    {
        return errorFlag_;
    }

    inline TokenLocation Scanner::getTokenLocation() const
    {
        return TokenLocation(fileName_, line_, column_);
    }
}

#endif // scanner.h