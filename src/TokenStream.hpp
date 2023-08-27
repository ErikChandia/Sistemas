#ifndef __TOKEN_STREAM__
#define __TOKEN_STREAM__
#include <std_lib_facilities.h>
#include <Token.hpp>

#define var      'v'
#define constant 'c'
#define nme      'a'
#define number   '8'
#define printer  ';'
#define exit     'q'
#define funct    'f'

const std::string varkey  = "let";
const std::string quit    = "exit";
const std::string squart  = "sqrt";
const std::string power   = "pow";
const std::string coseno  = "cos";
const std::string seno    = "sin";

class TokenStream {
    public:
        Token get();            // get a Token
        void unget(Token t);  // put a Token back
        void ignore(char c);    // discard characters up to and including a c
    private:
        bool full {false};      // is there a Token in the buffer?
        Token buffer {'0'};           // where we store a 'putback' Token
};

#endif