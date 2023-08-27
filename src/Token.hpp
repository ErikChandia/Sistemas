#ifndef __TOKEN__
#define __TOKEN__
#include <string>

class Token {
public:
    char  kind;
    double value;
    std::string name;
    double (*func1)(double&);

    Token() = default;
    Token(char ch) : kind(ch), value(0) { }                    // Make a Token from a char
    Token(char ch, double val) : kind(ch), value(val) { }      // Make a Token from a char and a double
    Token(char ch, std::string n) : kind{ch}, name{n} { }      // Initialize kind and name 
};

#endif