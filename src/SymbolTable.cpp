#include <SymbolTable.hpp>
#include <error.hpp>

void SymbolTable::set_value(const std::string& s, double d) // set the Variable named s to d
{
    for (Variable& v : var_table)
        if (v.name == s && !v.isConst) { v.value = d; return; }
    error("set: undefined 'variable' ", s); 
}

double SymbolTable::get_value(const std::string& s) const // return the value of the Variable named s
{
    for (const Variable& v : var_table)
        if (v.name == s) return v.value;
    error("get: undefined variable ", s);
    return 0;
}

bool SymbolTable::is_varDeclared(const std::string& name) const noexcept
{
    for (const Variable& v : var_table)
        if (v.name == name && !v.isConst) return true;
    return false;
}

bool SymbolTable::is_constDeclared(const std::string& name) const noexcept
{
    for (const Variable& v : var_table)
        if (v.name == name && v.isConst) return true;
    return false;
}

bool SymbolTable::is_declared(const std::string& name) const // is name already in var_table?
{
    for (const Variable& v : var_table)
        if (v.name == name) return true;
    return false;
}

double SymbolTable::define_var(const std::string& name, double val)  // add (name,val) to var_table
{
    bool varDecl   = is_varDeclared(name);
    bool constDecl = is_constDeclared(name);
    if (varDecl && !constDecl) {
        set_value(name, val);
        return val;
    }
    if (constDecl) error(name, " declared twice");
    var_table.push_back(Variable(name, val, false));
    return val;
}

double SymbolTable::define_const(const std::string& name, double val)  // add (name,val) to var_table
{
    if (is_declared(name)) error(name, " declared twice"); 
    var_table.push_back(Variable(name, val, true));
    return val;
}