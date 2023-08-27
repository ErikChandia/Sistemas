#include <SymbolTable.hpp>
#include <TokenStream.hpp>

/*
    Este programa implementa una calculadora de expresión básica. 
    Entrada de cin; salida a cout.
    La gramática para la entrada es:

        Calculation: 
            Statement
            Print
            Quit
            Calculation Statement   
        Statement: 
            Declaration
            Expression
        Declaration:
            "let" Name "=" Expression
        Expresión:
            Término
            Expresión "+" Término
            Expresión "–" Término
        Término:
            Primario
            Término "*" Primario 
            Término "/" Primario 
            Término "%" Primario
        Primario: 
            Número
            Número"!"
            "{(" Expresión ")}"
            "-" Primario
            "+" Primario
            Name                (variable or constant)
            Funct
        Número:
            punto flotante-literal
        Funct:
            sqrt( "{(" Expresión ")}",  "{(" Expresión ")}" );
            pow( "{(" Expresión ")}",  "{(" Expresión ")}" );
            cos( "{(" Expresión ")}" )
            sin( "{(" Expresión ")}" )
*/

// recuperar bandera buena de cin y limpiar buffer
// void cleanInputStream(){
//     cin.clear();
//     cin.ignore(100, '\n'); //numeric_limits<streamsize>::max() > 100
// }

double expression(TokenStream& ts);
double term(TokenStream& ts);
double primary(TokenStream& ts);

constexpr char prompt[3] = "> ";
constexpr char result[3] = "= ";

SymbolTable table;   // provides symbols(contants and variables)

double fact(int val){
    return val? val*fact(val-1) : 1; // hasta que no sea 0 o ya sea cero se devuelva 1.
}

double primary(TokenStream& ts)
{
    Token t = ts.get();

    switch (t.kind)
    {
    case '(': 
    {
        double expr = expression(ts);
        t = ts.get();
        if (t.kind != ')') error("')' expected");
        return expr;
    }
    case '{': 
    {
        double expr = expression(ts);
        t = ts.get();
        if (t.kind != '}') error("'}' expected");
        return expr;
    }
    case number:{                   // we use '8' to represent a number
        double value = t.value;
        t = ts.get();
        if (t.kind == '!') return fact(narrow_cast<int>(value));
        ts.unget(t);
        return value;
    }

    case nme:{
        return table.get_value(t.name);
    }
    
    case funct:{
        string nameFunc = t.name;
        t = ts.get();
        if (t.kind != '(') error("'(' expected");
        double expr = expression(ts);
        
        if (nameFunc == power){
            t = ts.get();
            if (t.kind != ',') error("',' expected");
            double expr2 = expression(ts);
            t = ts.get();
            if (t.kind != ')') error("')' expected");
            return pow(expr, expr2);
        }

        t = ts.get();
        if (t.kind != ')') error("')' expected");
        if (nameFunc == squart) return sqrt(expr);
        if (nameFunc == coseno) return cos(expr);
        if (nameFunc == seno)   return sin(expr);
    }

    case '-': return -primary(ts);

    case '+': return primary(ts);
    // case 'q':
    //     ts.putback(t);
    //     break;

    default:
        error("primary expected");
    }
    return 0;
}

double term(TokenStream& ts)
{
    double left = primary(ts);

    Token t;
    while(true)
    {
        t = ts.get();
        switch (t.kind)
        {
        case '*':
            left *= primary(ts);
            break;
        case '/':
        {
            double prim = primary(ts);
            if (prim == 0) error("divide by zero");
            left /= prim;
            break;
        }
        case '%':{
            //error("Module operator (%) no recognized");
            double prim = primary(ts);
            if (prim == 0) error("divide by zero");
            left = fmod(left, prim); // modulo punto flotante
            break;
        }
        default:
            ts.unget(t);
            return left;
        }
    }
}

double expression(TokenStream& ts)
{
    double left = term(ts);      // read and evaluate a Term

    Token t;
    while(true) 
    {
        t = ts.get();           // get the next token
        switch(t.kind) {
        case '+':
            left += term(ts);    // evaluate Term and add
            break;
        case '-':
            left -= term(ts);    // evaluate Term and subtract
            break;
        default:
            ts.unget(t);
            return left;       // finally: no more + or -: return the answer
        }
    }
}

// assume we have seen "let”
// handle: name = expression
// declare a variable called "name” with the initial value "expression”
double declaration(TokenStream& ts, bool isConst)
{
    Token t = ts.get();
    if (t.kind != nme) error ("name expected in declaration");
    string name = t.name;

    Token t2 = ts.get();
    if (t2.kind != '=') error("= missing in declaration of ", name);
    double d = expression(ts);
    
    if (isConst) return table.define_const(name, d);
    else         return table.define_var(name, d);
}

double statement(TokenStream& ts) 
{
    Token t = ts.get(); 

    switch (t.kind) {
        case constant: return declaration(ts, true);
        case var:      return declaration(ts, false); 
        default:  
          ts.unget(t); return expression(ts); 
    }
}

void calculate()
{
    Token t;
    TokenStream ts;      // provides get() and putback()
    double res = 0;

    while (cin)
    try{
        cout<<prompt;
        t = ts.get();

        while (t.kind == printer) t = ts.get();  // eat ‘;’
        if    (t.kind == exit)  break;
        ts.unget(t);

        res = statement(ts);
        cout<<result<<res<<'\n';
    }
    catch(const exception& e){
        std::cerr<<e.what()<<'\n';
        ts.ignore(printer);// limpia flujo de caracteres quedado en la expresion antes de un ;
    }
}

void define_constants()
{
    table.define_const("PI", 3.1415926535);
    table.define_const("E",  2.7182818284);
    table.define_const("k",          1000);
}

int main(void)
try {
    define_constants();
    calculate();
    keep_window_open();
    return 0;
    
} catch(const exception& e){
    cerr<<e.what()<<endl;
    return 1;
} catch(...){
    cerr<<"unknown exception"<<endl;
    return 2;
}