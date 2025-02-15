/*
---
E5.2
[2] Add the ability to use {} as well as () in the program, so that {(4+5)*6} /
(3+4) will be a valid expression.
---

This one was way easier than expected, but I don't see what mistakes there might be O_O

7 Nov 2024
@jmerort
*/

#include <iostream>



//------------------------------------------------------------------------------



class Token
{
    public:
        char kind; // what kind of token
        double value; // for numbers: a value

        Token(char ch):kind(ch), value(0) { } // make a Token from a char
        Token(char ch, double val):kind(ch), value(val) { } // make a Token from a char and a double
};



//------------------------------------------------------------------------------



class Token_stream 
{
    public:
        Token_stream(); // make a Token_stream that reads from cin
        Token get(); // get a Token (get() is defined elsewhere)
        void putback(Token t); // put a Token back

    private:
        bool full; // is there a Token in the buffer?
        Token buffer; // here is where we keep a Token put back using putback()
};



//------------------------------------------------------------------------------



// The constructor just sets full to indicate that the buffer is empty:

Token_stream::Token_stream():full(false), buffer(0) // no Token in buffer
{
}



//------------------------------------------------------------------------------



// The putback() member function puts its argument back into the Token_stream's buffer:

void Token_stream::putback(Token t)
{
    if (full) 
        throw(std::runtime_error("putback() into a full buffer"));

    buffer = t; // copy t to buffer
    full = true; // buffer is now full
}



//------------------------------------------------------------------------------



Token Token_stream::get()
{
    if (full) 
    { // do we already have a Token ready?
        // remove token from buffer
        full = false;
        return buffer;
    }

    char ch;
    std::cin >> ch; // note that >> skips whitespace (space, newline, tab, etc.)

    switch (ch) 
    {
        case '=': // for "print"
        case 'x': // for "quit"
        case '(': case ')': case '+': case '-': case '*': case '/': case '{': case '}':
            return Token(ch); // let each character represent itself

        case '.':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        {
            std::cin.putback(ch); // put digit back into the input stream

            double val;
            std::cin >> val; // read a floating-point number

            return Token('8', val); // let '8' represent "a number"
        }
        default:
            throw(std::runtime_error("Bad token"));
    }
}



//------------------------------------------------------------------------------



Token_stream ts; // provides get() and putback()



//------------------------------------------------------------------------------



double expression(); // declaration so that primary() can call expression()



//------------------------------------------------------------------------------



// deal with numbers and parentheses

double primary()

{
    Token t = ts.get();

    switch (t.kind) 
    {
        case '{': //handle '{' expression '}'
        {
            double b = expression();

            t = ts.get();
            if (t.kind != '}') 
                throw(std::runtime_error("'}' expected"));
            return b;
        }
        case '(': // handle '(' expression ')'
        {
            double d = expression();

            t = ts.get();
            if (t.kind != ')') 
                throw(std::runtime_error("')' expected"));
            return d;
        }
        case '8': // we use '8' to represent a number
            return t.value; // return the number's value
        default:
            throw(std::runtime_error("primary expected"));
    }
}



//------------------------------------------------------------------------------



// deal with *, /, and %

double term()
{
    double left = primary();
    Token t = ts.get(); // get the next token from token stream

    while (true) 
    {
        switch (t.kind) 
        {
            case '*':
                left *= primary();
                t = ts.get();
                break;
            case '/':
            {
                double d = primary();
                if (d == 0) 
                    throw(std::runtime_error("divide by zero"));
                left /= d;
                t = ts.get();
                break;
            }
            default:
                ts.putback(t); // put t back into the token stream
                return left;
        }
    }
}



//------------------------------------------------------------------------------



// deal with + and -

double expression()
{
    double left = term(); // read and evaluate a Term
    Token t = ts.get(); // get the next token from token stream

    while (true) 
    {
        switch (t.kind) 
        {
            case '+':
            left += term(); // evaluate Term and add
            t = ts.get();
            break;

            case '-':
            left -= term(); // evaluate Term and subtract
            t = ts.get();
            break;

            default:
            ts.putback(t); // put t back into the token stream
            return left; // finally: no more + or -: return the answer
        }
    }
}



//------------------------------------------------------------------------------



int main()
{
    std::cout << "Welcome to our simple calculator.\nPlease enter expressions using floating-point numbers or enter \"x\" to exit.\n"
              << "You can use the operators \"+\", \"-\", \"*\" and \"/\", and parentheses \"3*(1+2)\""
              << "to make calculations with positive numbers.\n"
              << "Put an equal sign at the end to see results e.g.:\"(1*0.5+1)/5 =\"\n";
    double val = expression();
    try
    {
        while (std::cin) 
        {
            Token t = ts.get();
            if (t.kind == 'x') 
                break; // 'x' for quit
            if (t.kind == '=') // '=' for "print now"
                std::cout << "=" << val << '\n';
            else
                ts.putback(t);

            val = expression();
        }
    }

    catch (std::exception& e)
    {
        std::cerr << "error: " << e.what() << '\n';
        return 1;
    }

    catch (...) 
    {
        std::cerr << "Oops: unknown exception!\n";
        return 2;
    }
}

//------------------------------------------------------------------------------