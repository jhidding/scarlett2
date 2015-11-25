#pragma once
#include "object.hh"
#include "../../common/misc/global.hh"

namespace Scarlett
{
    using Misc::Global;
    
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

    extern Global<Special> ignore;

    inline Special const *get_special(std::string const &name)
        { return Global<Special>::dir().at(name); }

    inline bool is_ignore(Ptr a)
        { return a == &ignore; }
}
