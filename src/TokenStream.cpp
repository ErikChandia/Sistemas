#include <TokenStream.hpp>

bool isFunction(const string& name){
    return (name == squart || name == power || name == coseno || name == seno);
}

constexpr bool isCharacterOfVar(char ch){
    return (isalpha(ch) || isdigit(ch) || ch == '_');
}

void TokenStream::unget(Token t)
{
    if (full) error("putback() into a full buffer");
    buffer = t;         // copy t to buffer
    full = true;        // buffer is now full
}

void TokenStream::ignore(char c) // c represents the kind of Token
{
    //si en el buffer existe el caracter de termino de expresion, entonces los posibles caracteres en el buffer de cin puede ser parte de otra expresion. 
    if (full && buffer.kind == c) { // first look in buffer:
        cout<<"\naca1\n";
        full = false;
        return; 
    }
    cout<<"\naca2\n";
    // se desarta los caracteres restantes en buffer de cin que eran parte de la expression erronea
    full = false;
    char ch;
    //cin.unget();                // posiacaso, ya que cuando se hace cin.get puede que el buffer este vacio, y lo esperado es que se finalize con salto de linea, por lo tanto se añade al buffer lo ultimo que se hizo get, osea un salto de linea
    while (cin.get(ch)) {       // now search input: char ch = 0;
        if (ch=='\n') return;
    }
}

Token TokenStream::get()
{
    if (full) {         // check if we already have a Token ready
        full = false;
        return buffer;
    }
    char ch;
    while(cin.get(ch) && isspace(ch)){       // si encuentra un salto de linea(si son otros espacios los salta), significa que devuelve token de print para avisar que se debe mostrar resultado.
        if (ch == '\n') return Token {printer};
    }

    switch (ch) {
        case printer:
        case '=':
        case '(': case ')':
        case '{': case '}':
        case '+': case '-':
        case '*': case '/':
        case '%': case '!':
        case ',':                //a character for separating arguments in any function
            return Token {ch};   // let each character represent itself
        case '.':                // a floating-point-literal can start with a dot
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        {
            cin.putback(ch);        // put digit back into the input stream
            double val;
            cin>>val;               // read a floating-point numbers
            return Token {number, val};
        }
        case '#':                   // declkey
            return Token {constant};
        default:
            if (isalpha(ch))
            {
                string s;
                s += ch;
                while (cin.get(ch) && isCharacterOfVar(ch)) s+=ch; // cin.get(char&): no ignora caracteres en blanco
                cin.unget();

                if (s == varkey)   return Token {var};
                if (s == quit)     return Token {exit};
                if (isFunction(s)) return Token {funct, s};
                
                return Token {nme, s}; // nme: nombre de la variable o constante
            }
        error("Bad token");
    }
    return Token{};
}