#pragma once
#include "object.hh"
#include "special.hh"

namespace Scarlett
{
    class Symbol: public Object
    {
    public:
        std::string const name;

        Symbol(std::string const &name_):
            name(name_) {}

        std::string repr() const
            { return name; }

        virtual bool eq(Ptr a) const
            { return equal(a); }

        virtual bool equal(Ptr a) const
            { return cast<Symbol>(a)->name == name; }
    };

    inline bool is_symbol(Ptr a)
        { return is_type<Symbol>(a); }

    inline string symbol_to_string(Ptr a)
        { return cast<Symbol>(a)->name; }
        
    inline Ptr operator"" _s(char const *a, size_t s)
        { if (a[0] == '#') return Ptr(new Special(a));
	  else return Ptr(new Symbol(a)); }
}
