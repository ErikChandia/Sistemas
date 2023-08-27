#ifndef __VARIABLE__
#define __VARIABLE__
#include <string>

class Variable {
public:
    std::string name;
    double value;
    bool isConst;
    Variable(const std::string& n, double v, bool isC) : name{n}, value{v}, isConst{isC} {}
};

#endif