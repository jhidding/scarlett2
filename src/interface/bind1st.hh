#pragma once
#include "../system/object.hh"
#include <functional>

namespace Scarlett
{
    /*! stores the first argument of a function
     *
     * Stores the first argument to a function to create
     * a function object that has one less parameter.
     * @param f some function
     * @param a argument
     */
    template <typename Ret, typename Arg, typename ...Rest>
    class bind_1st
    {
        typedef std::function<Ret (Arg, Rest...)> F;

        F       foo;
        Arg     a;

        public:
            bind_1st(F const &f, Arg a_): foo(f), a(a_) {}

            inline Ret operator()(Rest &&... r)
            {
                return foo(a, std::forward<Rest>(r)...);
            }
    };
}
