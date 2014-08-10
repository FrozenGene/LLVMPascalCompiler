/**********************************
* File:    token.h
*
* Author:  Wu Zhao
*
* Email:   wuzhaozju@gmail.com
*
* Date:    2014/07/27
*
* License: BSD
*********************************/

#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
#include <iostream>
#include <tuple>
#include <map>

namespace llvmpascal
{
    enum class TokenType
    {
        // see pascal standard 6.4

        // in fact, we can put these 5 types to one category
        // named constant. but I want to make it more cleaner.
        INTEGER,        // such as 3, 4 and so on
        REAL,           // such as 3.14
        BOOLEAN,        // true or false
        CHAR,           // such as 'a','b'
        STRING_LITERAL, // such as 'hello world'


        IDENTIFIER,     // such as abc
        KEYWORDS,       // such as if
        OPERATORS,      // such as  + - * /
        DELIMITER,      // such as ,
        END_OF_FILE,    // end of file
        UNKNOWN
    };

    enum class TokenValue
    {
        // see pascal standard 6.1.2
        AND,
        FOR,
        TO,
        DOWNTO,
        DO,
        IF,
        THEN,
        ELSE,
        WHILE,
        REPEAT,
        UNTIL,
        FUNCTION,
        PROCEDURE,
        BEGIN,
        END,
        PROGRAM,
        FORWARD,
        GOTO,
        OR,
        NOT,
        CASE,
        OTHERWISE,
        WITH,
        IN,

        // I/O routine
        WRITE,
        WRITELN,
        READ,
        READLN,

        // type/var/const
        TYPE,
        VAR,
        PACKED,
        ARRAY,
        OF,
        RECORD,
        CONST,
        FILE,
        SET,
        STRING,

        // symbols
        LEFT_PAREN,        // (
        RIGHT_PAREN,       // )
        LEFT_SQUARE,       // [
        RIGHT_SQUARE,      // ]
        PLUS,              // +
        MINUS,             // -
        MULTIPLY,          // *
        DIVIDE,            // /
        COMMA,             // ,
        SEMICOLON,         // ;
        COLON,             // :
        ASSIGN,            // :=
        PERIOD,            // .
        DOT_DOT,           // ..
        UPARROW,           // ^
        DIV,               // div
        MOD,               // mod
        XOR,               // xor
        SHR,               // shr
        SHL,               // shl

        // comparation symbols
        LESS_OR_EQUAL,     // <=
        LESS_THAN,         // <
        GREATER_OR_EQUAL,  // >=
        GREATER_THAN,      // >
        EQUAL,             // =
        NOT_EQUAL,         // <>

        UNRESERVED
    };


    class TokenLocation
    {
      public:
        TokenLocation();
        TokenLocation(const std::string& fileName, int line, int column);

        // this method is very similar with toString method in Java.
        std::string toString() const;
      private:
        std::string fileName_;
        int line_;
        int column_;
    };

    class Token
    {
      public:
        Token();
        Token(TokenType type, TokenValue value, const TokenLocation& location,
              std::string name, int symbolPrecedence);
        Token(TokenType type, TokenValue value, const TokenLocation& location,
              const std::string& strValue, std::string name);
        Token(TokenType type, TokenValue value, const TokenLocation& location,
              long intValue, std::string name);
        Token(TokenType type, TokenValue value, const TokenLocation& location,
              double realValue, std::string name);

        // get token information
        TokenType getTokenType() const;
        TokenValue getTokenValue() const;
        const TokenLocation& getTokenLocation() const;
        std::string getTokenName() const;

        // + - * / and so on.
        int getSymbolPrecedence() const;

        // get constant values of token
        long getIntValue() const;
        double getRealValue() const;
        std::string getStringValue() const;

        // output debug information.
        // here output token location, value and type.
        // we can control it using option(such as -debug).
        // but now, we use it directly.
        void dump(std::ostream& out = std::cout) const;


        std::string tokenTypeDescription() const;
        std::string toString() const;

      private:
        TokenType       type_;
        TokenValue      value_;
        TokenLocation   location_;
        int             symbolPrecedence_;
        std::string     name_;

        // const values of token
        long            intValue_;
        double          realValue_;
        std::string     strValue_;


    };

    inline TokenType Token::getTokenType() const
    {
        return type_;
    }

    inline TokenValue Token::getTokenValue() const
    {
        return value_;
    }

    inline std::string Token::getTokenName() const
    {
        return name_;
    }

    inline const TokenLocation& Token::getTokenLocation() const
    {
        return location_;
    }

    inline long Token::getIntValue() const
    {
        return intValue_;
    }

    inline double Token::getRealValue() const
    {
        return realValue_;
    }

    inline std::string Token::getStringValue() const
    {
        return strValue_;
    }

    inline int Token::getSymbolPrecedence() const
    {
        return symbolPrecedence_;
    }
}

#endif // token.h