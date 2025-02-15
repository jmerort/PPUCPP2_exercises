/*
---
8.13
[13] Design and implement a rational number class, Rational. A rational
number has two parts: a numerator and a denominator, for example, 5/6
(five-sixths, also known as approximately .83333). Look up the
definition if you need to. Provide assignment, addition, subtraction,
multiplication, division, and equality operators. Also, provide a
conversion to double. Why would people want to use a Rational class?
---

The compiler doesn't allow me to declare the operator= function as a friend of
the class, for some reason.

Jan 2024
@jmerort
*/


#include <iostream>

using std::cout;


//---------------------------------------------------------------------------



class Rational 
{
    public:
        Rational (int nn, int dd);

        int num(){return n;};
        int den(){return d;};

		double to_double(); //convert rational to double

		void operator= (Rational&); //assignment operator

		friend Rational operator+ (Rational&, Rational&); //arithmetic operators
		friend Rational operator- (Rational&, Rational&); 
		friend Rational operator* (Rational&, Rational&);
		friend Rational operator/ (Rational&, Rational&);

        friend bool operator== (Rational&, Rational&); //equality operator
		friend bool operator!= (Rational&, Rational&);

        friend std::ostream& operator << (std::ostream&, Rational); //output operator

		struct Invalid {};

    private:
        int n,
            d;

		void simplify();
};



Rational::Rational (int nn, int dd):n{nn},d{dd}
{
	if (dd == 0) throw Invalid {};
	//simplify(); //if you want to simplify every rational
}



void Rational::simplify()
{
	for (int i {2}; i <= n || i <= d; i++)
	{
		if (n % i == 0 && d % i == 0)
		{
			d = d / i;
			n = n / i;
			i = 1; //start counting again
		}
	}
}



double Rational::to_double()
{
	double nn {n},
		   dd {d};
	return nn / dd;
}



void Rational::operator=(Rational& a)
{
	n = a.n;
	d = a.d;
}



Rational operator+ (Rational& a, Rational& b)
{
	int den {a.den()*b.den()},
	    num {a.num()*b.den() + b.num()*a.den()};

	Rational sum {num, den};
	sum.simplify();

	return sum;
}



Rational operator- (Rational& a, Rational& b)
{
	int den {a.den()*b.den()},
	    num {a.num()*b.den() - b.num()*a.den()};

	Rational sub {num, den};
	sub.simplify();

	return sub;
}



Rational operator* (Rational& a, Rational& b)
{
	int den {a.den()*b.den()},
	    num {a.num()*b.num()};

	Rational mul {num, den};
	mul.simplify();

	return mul;
}



Rational operator/ (Rational& a, Rational& b)
{
	if (b.num() == 0) throw Rational::Invalid {}; //prevent division by zero

	int den {b.num()*a.den()},
	    num {a.num()*b.den()};

	Rational mul {num, den};
	mul.simplify();

	return mul;
}



bool operator== (Rational& a, Rational& b)
{ 
    return a.num()/a.den() == b.num()/b.den();
}



bool operator!= (Rational& a, Rational& b)
{
	return !(a == b);
}



std::ostream& operator << (std::ostream& os, Rational r)
{
	os << r.num() << '/' << r.den();
	return os;
}



//---------------------------------------------------------------------------



int main()
{
	try
	{
        Rational a {2,2},
                 b {5,15},
				 c {4,8},
				 d {8,16};
        
		//arithmetic
		cout << "a is " << a << " and b is " << b << '\n';
		cout << "a + b is " << a + b << '\n';
		cout << "a - b is " << a - b << '\n';
		cout << "a * b is " << a * b << '\n';
		cout << "a / b is " << a / b << '\n';

		//equality
		if (c == d) cout << c << " is equal to " << d << '\n';

		//double conversion
		cout << d << " as a double is " << d.to_double() << '\n';

		//assignment
		a = b; 
		cout << "a is now " << b << '\n';

		return 0;
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	catch(Rational::Invalid)
	{
		std::cerr << "Denominator can't be zero.\n";
		return 2;
	}
	catch(...)
	{
		std::cerr << "Unknown error.\n";
		return 3;
	}

}