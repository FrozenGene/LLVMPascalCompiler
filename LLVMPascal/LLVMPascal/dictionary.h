/**********************************
* File:    dictionary.h
*
* Author:  Wu Zhao
*
* Email:   wuzhaozju@gmail.com
*
* Date:    2014/07/30
*
* License: BSD
*********************************/

#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <string>
#include <tuple>
#include <map>
#include "token.h"

namespace llvmpascal
{
    class Dictionary
    {
      public:
        Dictionary();
        std::tuple<TokenType, TokenValue, int> lookup(const std::string& name) const;
        bool Dictionary::haveToken(const std::string& name) const;
      private:
        void addToken(std::string name, std::tuple<TokenValue, TokenType, int> tokenMeta);

      private:
        // four token property: token name, token value, token type, precedence.
        std::map<std::string, std::tuple<TokenValue, TokenType, int>> dictionary_;
    };
}

#endif // dictionary.h