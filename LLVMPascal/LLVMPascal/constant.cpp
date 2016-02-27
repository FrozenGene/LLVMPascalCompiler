/**********************************
* File:    constant.cpp
*
* Author:  Wu Zhao
* 
* Email:   wuzhaozju@gmail.com
* 
* Date:    2015/09/28
*
* License: BSD
*********************************/

#include "constant.h"

namespace llvmpascal
{

    Constant::Constant(ConstantKind kind, const TokenLocation& loc)
        : constantKind_(kind), tokenLocation_(loc)
    {}

    IntegerConstant::IntegerConstant(long l, const TokenLocation& loc)
        : Constant(ConstantKind::INTEGER_CONSTANT, loc), value_(l)
    {}

    Token IntegerConstant::makeToken() const
    {
        return Token(TokenType::INTEGER, TokenValue::UNRESERVED, tokenLocation_, value_, std::to_string(value_));
    }

    RealConstant::RealConstant(double d, const TokenLocation& loc)
        : Constant(ConstantKind::REAL_CONSTANT, loc), value_(d)
    {}

    Token RealConstant::makeToken() const
    {
        return Token(TokenType::REAL, TokenValue::UNRESERVED, tokenLocation_, value_, std::to_string(value_));
    }

    CharConstant::CharConstant(char c, const TokenLocation& loc)
        : Constant(ConstantKind::CHAR_CONSTANT, loc), value_(c)
    {}

    Token CharConstant::makeToken() const
    {
        // we use integer token constructor as we do in the scanner implementation
        return Token(TokenType::CHAR, TokenValue::UNRESERVED, tokenLocation_, static_cast<long>(value_), std::to_string(value_));
    }

    BoolConstant::BoolConstant(bool b, const TokenLocation& loc)
        : Constant(ConstantKind::BOOL_CONSTANT, loc), value_(b)
    {}

    Token BoolConstant::makeToken() const
    {
        // pascal standard see the literal is True and False,
        // but pascal is not case sensetive and we transform into
        // lower case as we do in the scanner implementation.
        // see pascal standard 6.9.3.5
        std::string boolLiteral = value_ ? "true" : "false";
        return Token(TokenType::IDENTIFIER, TokenValue::UNRESERVED, tokenLocation_, boolLiteral, std::to_string(value_));
    }

    StringConstant::StringConstant(const std::string& str, const TokenLocation& loc)
        : Constant(ConstantKind::STRING_CONSTANT, loc), value_(str)
    {}

    Token StringConstant::makeToken() const
    {
        return Token(TokenType::IDENTIFIER, TokenValue::UNRESERVED, tokenLocation_, value_, value_);
    }

    // Dump informatation to help to debug.
    void IntegerConstant::dump() const
    {
        std::cout << "Integer Constant: " << getValue() << std::endl;
    }

    void RealConstant::dump() const
    {
        std::cout << "Real Constant: " << getValue() << std::endl;
    }

    void CharConstant::dump() const
    {
        std::cout << "Char Constant: " << getValue() << std::endl;
    }

    void BoolConstant::dump() const
    {
        std::cout << "Bool Constant: " << getValue() << std::endl;
    }

    void StringConstant::dump() const
    {
        std::cout << "String Constant: " << getValue() << std::endl;
    }


}