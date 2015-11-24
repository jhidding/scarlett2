#pragma once
#include "object.hh"

namespace Scarlett
{
    class Special: public Object
    {
    public:
        std::string const name;

        Special(std::string const &name_):
            name(name_) {}

        /* Type const *type() const; */

        std::string repr() const
            { return "#"+name; }

        virtual bool eq(Ptr a) const
            { return equal(a); }

        virtual bool equal(Ptr a) const
            { return cast<Special>(a)->name == name; }
    };

    inline bool is_special(Ptr a)
        { return is_type<Special>(a); }
}
