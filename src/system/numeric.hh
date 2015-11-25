#pragma once
#include "atom.hh"
#include <complex>
#include "../numeric/rational.hh"

namespace Scarlett
{
    using Numeric::rational;

    class NumericType: public Type
    {
        bool _exact;

    public:
        NumericType(std::string const &name, bool exact_):
            Type(name), _exact(exact_) {}

        bool is_exact() const
            { return _exact; }
    };

    class Numeric
    {
    public:
        virtual double to_double() const
            { throw Exception(ERROR_type,
                "This numeric type has no meaningfull real representation."); }
    };

    class Infinity: public Object, public Numeric
    {
        int sign;

    public:
        virtual double to_double() const
        {
            return sign * std::numeric_limits<double>::infinity();
        }

        virtual Type const *type() const;
    };

    template <>
    class Atom<int>: public AtomBase<int>, public Numeric
    {
    public:
        using AtomBase<int>::AtomBase;

        virtual double to_double() const
            { return static_cast<double>(this->value); }

        virtual Type const *type() const;
    };

    template <>
    class Atom<double>: public AtomBase<double>, public Numeric
    {
    public:
        using AtomBase<double>::AtomBase;

        virtual double to_double() const
            { return this->value; }

        virtual Type const *type() const;
    };

    template <>
    class Atom<rational>: public AtomBase<rational>, public Numeric
    {
    public:
        using AtomBase<rational>::AtomBase;

        virtual double to_double() const
            { return this->value.to_double(); }

        virtual Type const *type() const;
    };

    template <>
    class Atom<std::complex<double>>: public AtomBase<std::complex<double>>, public Numeric
    {
    public:
        using AtomBase<std::complex<double>>::AtomBase;
    };

    typedef Atom<std::complex<double>> Complex;
    
    inline Ptr make_polar(Ptr a_, Ptr b_)
    {
        double a = cast<Numeric>(a_)->to_double(),
               b = cast<Numeric>(b_)->to_double();
        return new Complex(std::polar(a, b));
    }

    inline Ptr make_rectangular(Ptr a_, Ptr b_)
    {
        double a = cast<Numeric>(a_)->to_double(),
               b = cast<Numeric>(b_)->to_double();
        return new Complex(std::complex<double>(a,b));
    }

    inline Ptr operator"" _a(unsigned long long a) { return Ptr(new Atom<int>(a)); }
}
