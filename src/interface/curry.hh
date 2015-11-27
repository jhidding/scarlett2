/*  This file is part of Scarlett.

    Scarlett is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Scarlett is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Scarlett.  If not, see <http://www.gnu.org/licenses/>. */

#include "../system/object.hh"

#include <functional>
#include "convert.hh"
#include "bind1st.hh"

namespace Scarlett
{
    /*! Interfaces C++ functions to Scarlett by curry.
     *
     * Has at least one template argument
     * @param Ret return type
     * @param Args... parameter types
     *
     * The Curry<> object behaves as a sequence of single parameter
     * functions; every function in the sequence returning another
     * Curry<> of one parameter less. The call operator is overloaded
     * to behave like a Scarlett \link CEO: the first argument is the
     * \link continuation, the second the \link environment.  The third
     * argument a Scarlett list containing the call arguments. Example:
     * \code{.cpp}
     * int fun(double a, std::string const &b) { ... }
     *
     * CEO Foo = Curry<int, double, std::string const &>(fun);
     * \endcode
     */
    template <typename Ret, typename ...Args>
    class Curry;

    template <typename Ret>
    class Curry<Ret>
    {
        typedef std::function<Ret ()> F;
        F    foo;

        public:
            Curry(F const &f): foo(f)
            {}

            inline Continuation *apply(Continuation *C, ptr args)
            {
                return C->give(Convert<Ret>::to_ptr(foo()));
            }

            inline Ret call(ptr args)
            {
                return foo();
            }
    };

    template <typename Ret, typename Arg, typename ...Rest>
    class Curry<Ret, Arg, Rest...>
    {
        typedef std::function<Ret (Arg, Rest...)> F;
        F    foo;

        public:
            Curry(F const &f): foo(f)
            {}

            inline Continuation *apply(Continuation *C,  ptr args)
            {
                if (sizeof...(Rest) != (proper_list_length(args) - 1))
                    throw Exception(ERROR, "wrong number of arguments");

                return C->give(Convert<Ret>::to_ptr(call(args)));
            }

            inline Ret call(ptr args)
            {
                return Curry<Ret, Rest...>(
                    bind_1st<Ret, Arg, Rest...>(
                        foo, Convert<Arg>::from_ptr(
                            car(args)))).call(cdr(args));
            }
    };
}
