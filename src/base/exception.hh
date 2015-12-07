#pragma once
#include "../../common/misc/utility.hh"
#include <iostream>
#include <exception>

namespace Scarlett
{
    using Misc::format;

    enum ErrNo { ERROR, ERROR_memory, ERROR_bound, ERROR_unbound, ERROR_match,
                 ERROR_type, ERROR_fail, ERROR_syntax, ERROR_ni,
                 ERROR_runtime, ERROR_lookup };

    class Exception: public std::exception
    {
        ErrNo         id;
        std::string descr;

        public:
            template <typename ...Args>
            Exception(ErrNo id_, Args &&...args):
                id(id_),
                descr(format(std::forward<Args>(args)...))
            {}

            bool is(ErrNo x) const { return x == id; }
            std::string const &description() const { return descr; }

            char const *what() const throw() { return descr.c_str(); }
    };

    inline std::ostream &operator<<(std::ostream &out, Exception const &E)
    {
        out << E.description();
        return out;
    }
}

// vim:sw=4:ts=4:tw=72
