#pragma once
#include <iostream>

namespace Numeric
{
	/*! \brief compute the greatest common denominator.
	 *  \param a integer
	 *  \param b integer
	 *  \return The largest integer dividing both \a a and \a b.
	 */
	extern int gcd_2(int a, int b);

	/*! \brief implements rational numbers
	 * Stores a fractional number as two integers \a a, \a b.
	 * With each operation, the fraction is simplified,
	 * so that \a a and \a b are coprime.
	 */
	class rational
	{
		int a, b;

		public:
			rational() {}
			rational(int a_, int b_ = 1);
			void simplify();
			rational &operator+=(rational const &o);
			rational &operator-=(rational const &o);
			rational &operator*=(rational const &o);
			rational &operator/=(rational const &o);
			rational &operator*=(int s);
			rational &operator/=(int s);
			rational operator+(rational const &o) const;
			rational operator-(rational const &o) const;
			rational operator*(rational const &o) const;
			rational operator*(int s) const;
			rational operator/(rational const &o) const;
			rational square_scale(rational const &o) const;
			rational operator-() const;

			int integer() const;
			rational fraction() const;

			bool operator==(rational const &o) const;
			bool operator==(int o) const;
			bool operator<(rational const &o) const;
			bool operator>(rational const &o) const;
			bool operator>=(rational const &o) const;
			bool operator<=(rational const &o) const;
			bool operator!=(rational const &o) const;

			int num() const { return a; }
			int den() const { return b; }

            double to_double() const { return double(a)/b; }
	};

	inline rational operator*(int a, rational const &b)
	{
		return b*a;
	}

	inline std::ostream &operator<<(std::ostream &out, rational const &r)
	{
		if (r.den() == 1)
		{
			return out << r.num();
		}

		if (r.num() > r.den())
		{
			auto d = std::div(r.num(), r.den());
			return out << d.quot << "+" << d.rem << "/" << r.den();
		}

		return out << r.num() << "/" << r.den();
	}

	/*! \brief shorthand to make a rational number.
	 */
	inline rational _r(int a, int b = 1)
	{
		return rational(a, b);
	}
}

/* vim: set si */
