/**********************************
* File:    token.cpp
*
* Author:  Wu Zhao
*
* Email:   wuzhaozju@gmail.com
*
* Date:    2014/07/27
*
* License: BSD
*********************************/

#include "token.h"


namespace llvmpascal
{

    TokenLocation::TokenLocation(const std::string& fileName, int line, int column)
        : fileName_(fileName), line_(line), column_(column)
    {}

    TokenLocation::TokenLocation() : fileName_(""), line_(1), column_(0)
    {}


    std::string TokenLocation::toString() const
    {
        return fileName_ + ":" + std::to_string(line_) + ":" + std::to_string(column_) + ":";
    }

    // End TokenLocation


    Token::Token() : type_(TokenType::UNKNOWN), value_(TokenValue::UNRESERVED),
        location_(std::string(""), 0, 0), name_(""), symbolPrecedence_(-1)
    {}

    Token::Token(TokenType type, TokenValue value, const TokenLocation& location,
                 std::string name, int symbolPrecedence)
        : type_(type), value_(value), location_(location), name_(name),
          symbolPrecedence_(symbolPrecedence)
    {}

    Token::Token(TokenType type, TokenValue value, const TokenLocation& location,
                 const std::string& strValue, std::string name)
        : type_(type), value_(value), location_(location),
          strValue_(strValue), name_(name), symbolPrecedence_(-1)
    {}

    Token::Token(TokenType type, TokenValue value, const TokenLocation& location,
                 long intValue, std::string name)
        : type_(type), value_(value), location_(location),
          intValue_(intValue), name_(name), symbolPrecedence_(-1)
    {}

    Token::Token(TokenType type, TokenValue value, const TokenLocation& location,
                 double realValue, std::string name)
        : type_(type), value_(value), location_(location),
          realValue_(realValue), name_(name), symbolPrecedence_(-1)
    {}

    std::string Token::tokenTypeDescription() const
    {
        std::string buffer;

        switch (type_)
        {
            case llvmpascal::TokenType::INTEGER:
                buffer = "integer:        ";
                break;

            case llvmpascal::TokenType::REAL:
                buffer = "real:           ";
                break;

            case llvmpascal::TokenType::BOOLEAN:
                buffer = "boolean:        ";
                break;

            case llvmpascal::TokenType::CHAR:
                buffer = "char:           ";
                break;

            case llvmpascal::TokenType::STRING_LITERAL:
                buffer = "string literal: ";
                break;

            case llvmpascal::TokenType::IDENTIFIER:
                buffer = "identifier:     ";
                break;

            case llvmpascal::TokenType::KEYWORDS:
                buffer = "keywords:       ";
                break;

            case llvmpascal::TokenType::OPERATORS:
                buffer = "operators:      ";
                break;

            case llvmpascal::TokenType::DELIMITER:
                buffer = "delimiter:      ";
                break;

            case llvmpascal::TokenType::END_OF_FILE:
                buffer = "eof             ";
                break;

            case llvmpascal::TokenType::UNKNOWN:
                buffer = "unknown:        ";
                break;

            default:
                break;
        }

        return buffer;
    }

    void Token::dump(std::ostream& out /* = std::cout */) const
    {
        out << location_.toString() << "\t" << tokenTypeDescription()
            << "\t" << name_ << "\t\t" << getSymbolPrecedence() << std::endl;
    }
    // End Token

}
