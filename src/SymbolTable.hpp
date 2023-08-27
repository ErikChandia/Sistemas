#ifndef __SYMBOL_TABLE__
#define __SYMBOL_TABLE__
#include <string>
#include <vector>
#include <Variable.hpp>

class SymbolTable {
public:
    double get_value(const std::string& s) const;
    double define_var(const std::string& name, double val);
    double define_const(const std::string& name, double val);

private:
    bool is_declared(const std::string& name)      const;
    bool is_varDeclared(const std::string& name)   const noexcept;
    bool is_constDeclared(const std::string& name) const noexcept;
    void set_value(const std::string& s, double d);

    std::vector<Variable> var_table;
};

#endif