/**********************************
* File:    dictionary.cpp
*
* Author:  Wu Zhao
*
* Email:   wuzhaozju@gmail.com
*
* Date:    2014/07/30
*
* License: BSD
*********************************/
#include "dictionary.h"

namespace llvmpascal
{
    Dictionary::Dictionary()
    {
        addToken(":=",           std::make_tuple(TokenValue::ASSIGN,           TokenType::OPERATORS, 0));
        addToken("=",            std::make_tuple(TokenValue::EQUAL,            TokenType::OPERATORS, 1));
        addToken("<>",           std::make_tuple(TokenValue::NOT_EQUAL,        TokenType::OPERATORS, 2));
        addToken(">=",           std::make_tuple(TokenValue::GREATER_OR_EQUAL, TokenType::OPERATORS, 2));
        addToken(">",            std::make_tuple(TokenValue::GREATER_THAN,     TokenType::OPERATORS, 2));
        addToken("<=",           std::make_tuple(TokenValue::LESS_OR_EQUAL,    TokenType::OPERATORS, 2));
        addToken("<",            std::make_tuple(TokenValue::LESS_THAN,        TokenType::OPERATORS, 2));
        addToken("+",            std::make_tuple(TokenValue::PLUS,             TokenType::OPERATORS, 10));
        addToken("-",            std::make_tuple(TokenValue::MINUS,            TokenType::OPERATORS, 10));
        addToken("*",            std::make_tuple(TokenValue::MULTIPLY,         TokenType::OPERATORS, 20));
        addToken("/",            std::make_tuple(TokenValue::DIVIDE,           TokenType::OPERATORS, 20));
        addToken(":",            std::make_tuple(TokenValue::COLON,            TokenType::DELIMITER, -1));
        addToken(",",            std::make_tuple(TokenValue::COMMA,            TokenType::DELIMITER, -1));
        addToken("..",           std::make_tuple(TokenValue::DOT_DOT,          TokenType::DELIMITER, -1));
        addToken("(",            std::make_tuple(TokenValue::LEFT_PAREN,       TokenType::DELIMITER, -1));
        addToken("[",            std::make_tuple(TokenValue::LEFT_SQUARE,      TokenType::DELIMITER, -1));
        addToken(".",            std::make_tuple(TokenValue::PERIOD,           TokenType::DELIMITER, -1));
        addToken(")",            std::make_tuple(TokenValue::RIGHT_PAREN,      TokenType::DELIMITER, -1));
        addToken("]",            std::make_tuple(TokenValue::RIGHT_SQUARE,     TokenType::DELIMITER, -1));
        addToken(";",            std::make_tuple(TokenValue::SEMICOLON,        TokenType::DELIMITER, -1));
        addToken("^",            std::make_tuple(TokenValue::UPARROW,          TokenType::DELIMITER, -1));
        addToken("and",          std::make_tuple(TokenValue::AND,              TokenType::KEYWORDS,  -1));
        addToken("array",        std::make_tuple(TokenValue::ARRAY,            TokenType::KEYWORDS,  -1));
        addToken("begin",        std::make_tuple(TokenValue::BEGIN,            TokenType::KEYWORDS,  -1));
        addToken("case",         std::make_tuple(TokenValue::CASE,             TokenType::KEYWORDS,  -1));
        addToken("const",        std::make_tuple(TokenValue::CONST,            TokenType::KEYWORDS,  -1));
        addToken("do",           std::make_tuple(TokenValue::DO,               TokenType::KEYWORDS,  -1));
        addToken("downto",       std::make_tuple(TokenValue::DOWNTO,           TokenType::KEYWORDS,  -1));
        addToken("else",         std::make_tuple(TokenValue::ELSE,             TokenType::KEYWORDS,  -1));
        addToken("end",          std::make_tuple(TokenValue::END,              TokenType::KEYWORDS,  -1));
        addToken("file",         std::make_tuple(TokenValue::FILE,             TokenType::KEYWORDS,  -1));
        addToken("for",          std::make_tuple(TokenValue::FOR,              TokenType::KEYWORDS,  -1));
        addToken("forward",      std::make_tuple(TokenValue::FORWARD,          TokenType::KEYWORDS,  -1));
        addToken("function",     std::make_tuple(TokenValue::FUNCTION,         TokenType::KEYWORDS,  -1));
        addToken("if",           std::make_tuple(TokenValue::IF,               TokenType::KEYWORDS,  -1));
        addToken("goto",         std::make_tuple(TokenValue::GOTO,             TokenType::KEYWORDS,  -1));
        addToken("of",           std::make_tuple(TokenValue::OF,               TokenType::KEYWORDS,  -1));
        addToken("otherwise",    std::make_tuple(TokenValue::OTHERWISE,        TokenType::KEYWORDS,  -1));
        addToken("packed",       std::make_tuple(TokenValue::PACKED,           TokenType::KEYWORDS,  -1));
        addToken("procedure",    std::make_tuple(TokenValue::PROCEDURE,        TokenType::KEYWORDS,  -1));
        addToken("program",      std::make_tuple(TokenValue::PROGRAM,          TokenType::KEYWORDS,  -1));
        addToken("read",         std::make_tuple(TokenValue::READ,             TokenType::KEYWORDS,  -1));
        addToken("readln",       std::make_tuple(TokenValue::READLN,           TokenType::KEYWORDS,  -1));
        addToken("record",       std::make_tuple(TokenValue::RECORD,           TokenType::KEYWORDS,  -1));
        addToken("repeat",       std::make_tuple(TokenValue::REPEAT,           TokenType::KEYWORDS,  -1));
        addToken("set",          std::make_tuple(TokenValue::SET,              TokenType::KEYWORDS,  -1));
        addToken("string",       std::make_tuple(TokenValue::STRING,           TokenType::KEYWORDS,  -1));
        addToken("then",         std::make_tuple(TokenValue::THEN,             TokenType::KEYWORDS,  -1));
        addToken("to",           std::make_tuple(TokenValue::TO,               TokenType::KEYWORDS,  -1));
        addToken("type",         std::make_tuple(TokenValue::TYPE,             TokenType::KEYWORDS,  -1));
        addToken("until",        std::make_tuple(TokenValue::UNTIL,            TokenType::KEYWORDS,  -1));
        addToken("var",          std::make_tuple(TokenValue::VAR,              TokenType::KEYWORDS,  -1));
        addToken("while",        std::make_tuple(TokenValue::WHILE,            TokenType::KEYWORDS,  -1));
        addToken("writeln",      std::make_tuple(TokenValue::WRITELN,          TokenType::KEYWORDS,  -1));
        addToken("in",           std::make_tuple(TokenValue::IN,               TokenType::KEYWORDS,  2));
        addToken("or",           std::make_tuple(TokenValue::OR,               TokenType::KEYWORDS,  10));
        addToken("xor",          std::make_tuple(TokenValue::XOR,              TokenType::KEYWORDS,  10));
        addToken("div",          std::make_tuple(TokenValue::DIV,              TokenType::KEYWORDS,  20));
        addToken("shl",          std::make_tuple(TokenValue::SHL,              TokenType::KEYWORDS,  20));
        addToken("shr",          std::make_tuple(TokenValue::SHR,              TokenType::KEYWORDS,  20));
        addToken("mod",          std::make_tuple(TokenValue::MOD,              TokenType::KEYWORDS,  20));
        addToken("not",          std::make_tuple(TokenValue::NOT,              TokenType::KEYWORDS,  40));
    }

    void Dictionary::addToken(std::string name,
                              std::tuple<TokenValue, TokenType, int> tokenMeta)
    {
        dictionary_.insert(std::pair<decltype(name), decltype(tokenMeta)>(name, tokenMeta));
    }

    // if we can find it in the dictionary, we change the token type
    std::tuple<TokenType, TokenValue, int> Dictionary::lookup(const std::string& name) const
    {
        TokenValue tokenValue = TokenValue::UNRESERVED;
        TokenType  tokenType  = TokenType::IDENTIFIER;
        int        precedence = -1;
        auto iter = dictionary_.find(name);

        if (iter != dictionary_.end())
        {
            tokenValue = std::get<0>(iter->second);
            tokenType  = std::get<1>(iter->second);
            precedence = std::get<2>(iter->second);
        }

        return std::make_tuple(tokenType, tokenValue, precedence);
    }

    bool Dictionary::haveToken(const std::string& name) const
    {
        auto iter = dictionary_.find(name);

        if (iter != dictionary_.end())
        {
            return true;
        }

        return false;
    }

}