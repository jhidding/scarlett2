#pragma once
#include "object.hh"

namespace Scarlett
{
    template <typename T>
    class AtomBase: public Object
    {
    public:
        T value;

        AtomBase(T const &v): value(v) {}

        virtual std::string repr() const
            { return format(value); }

        virtual bool equal(Ptr a) const
            { return cast<AtomBase<T>>(a)->value == value; }
    };

    class Numeric
    {
    public:
        virtual double get_double() const = 0;
    };

    template <typename T>
    class Atom: public AtomBase<T>
    {
    public:
        using AtomBase<T>::AtomBase;
    };

    template <>
    class Atom<int>: public AtomBase<int>, public Numeric
    {
    public:
        using AtomBase<int>::AtomBase;

        virtual double get_double() const
            { return static_cast<double>(this->value); }
    };

    template <>
    class Atom<double>: public AtomBase<double>, public Numeric
    {
    public:
        using AtomBase<double>::AtomBase;

        virtual double get_double() const
            { return this->value; }
    };

    template <>
    class Atom<bool>: public AtomBase<bool>
    {
    public:
        using AtomBase<bool>::AtomBase;

        virtual std::string repr() const
            { return (this->value ? "#t" : "#f"); }
    };

    typedef Atom<bool> Boolean;

    template <>
    class Atom<std::string>: public AtomBase<std::string>
    {
    public:
        using AtomBase<std::string>::AtomBase;

        virtual std::string repr() const
            { return "\"" + this->value + "\""; }
    };

    typedef Atom<std::string> String;

    inline Ptr operator"" _a(unsigned long long a) { return Ptr(new Atom<int>(a)); }
    inline Ptr operator"" _a(char const *a, size_t s) { return Ptr(new Atom<std::string>(a)); }
}
