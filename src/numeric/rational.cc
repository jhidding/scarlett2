#include "rational.hh"
#include <algorithm>
#include <cstdlib>

using namespace Numeric;

int Numeric::gcd_2(int a, int b)
{
	if (a == 0) return abs(b);
	if (b == 0) return abs(a);

	int x = 1; a = abs(a); b = abs(b);

	while (x != 0)
	{
		x = abs(a - b);
		a = std::min(a, b);
		b = x;
	}
	return a;
}

Numeric::rational::rational(int a_, int b_):
	a(a_), b(b_)
{
	simplify();
}

void Numeric::rational::simplify()
{
	int m = gcd_2(a, b);
	if (m == 0) return;
	a /= m; b /= m;
	if (b < 0) { a = -a; b = -b; }
}

rational &Numeric::rational::operator+=(rational const &o)
{
	a = a * o.b + o.a * b;
	b = b * o.b;
	simplify();
	return *this;
}

rational &Numeric::rational::operator-=(rational const &o)
{
	a = a * o.b - o.a * b;
	b = b * o.b;
	simplify();
	return *this;
}

rational &Numeric::rational::operator*=(rational const &o)
{
	a *= o.a;
	b *= o.b;
	simplify();
	return *this;
}

rational &Numeric::rational::operator/=(rational const &o)
{
	a *= o.b;
	b *= o.a;
	simplify();
	return *this;
}

rational &Numeric::rational::operator*=(int s)
{
	a *= s;
	simplify();
	return *this;
}

rational Numeric::rational::operator*(int s) const
{
	return rational(*this) *= s;
}

rational &Numeric::rational::operator/=(int s)
{
	b *= s;
	simplify();
	return *this;
}

rational Numeric::rational::operator+(rational const &o) const
{
	return rational(
		a * o.b + b * o.a,
		b * o.b);
}

rational Numeric::rational::operator-(rational const &o) const
{
	return rational(
		a * o.b - b * o.a,
		b * o.b);
}

rational Numeric::rational::operator-() const
{
	return rational(-a, b);
}

rational Numeric::rational::operator*(rational const &o) const
{
	return rational(a * o.a, b * o.b);
}

rational Numeric::rational::operator/(rational const &o) const
{
	return rational(a * o.b, b * o.a);
}

//Numeric::rational::operator int() const { return a/b; }
//Numeric::rational::operator double() const { return double(a)/b; }

rational Numeric::rational::square_scale(rational const &o) const
{
	return rational(a * o.a*o.a, b * o.b*o.b);
}

bool Numeric::rational::operator==(rational const &o) const
{
	return a == o.a and b == o.b;
}

bool Numeric::rational::operator==(int o) const
{
	return (b == 1) and (a == o);
}

bool Numeric::rational::operator<(rational const &o) const
{
	rational q = *this - o;
	return q.a < 0;
}

bool Numeric::rational::operator>(rational const &o) const
{
	rational q = *this - o;
	return q.a > 0;
}

bool Numeric::rational::operator>=(rational const &o) const
{
	rational q = *this - o;
	return q.a >= 0;
}

bool Numeric::rational::operator<=(rational const &o) const
{
	rational q = *this - o;
	return q.a <= 0;
}

bool Numeric::rational::operator!=(rational const &o) const
{
	return a != o.a or b != o.b;
}

int Numeric::rational::integer() const
{
	return a/b;
}

rational Numeric::rational::fraction() const
{
	return *this - rational(a/b);
}
