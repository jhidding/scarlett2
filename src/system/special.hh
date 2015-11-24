#pragma once
#include "object.hh"

namespace Scarlett
{
    class Special: public Object
    {
        std::string const _name;

    public:
        Special(std::string const &name_):
            _name(name_) {}

        std::string const &name() const
            { return _name; }

        std::string repr() const
            { return "#"+_name; }

        virtual bool eq(Ptr a) const
            { return equal(a); }

        virtual bool equal(Ptr a) const
            { return cast<Special>(a)->name() == name(); }
    };

    inline bool is_special(Ptr a)
        { return is_type<Special>(a); }

    inline bool is_ignore(Ptr a)
        { return is_special(a) and cast<Special>(a)->name() == "ignore"; }
}
