#pragma once
#include <utility>

namespace Scarlett
{
    /*! Universal object factory
     *
     * Makes a function of an object constructor. The result
     * can be passed to Curry<Type, Args...>
     * @param Type a class that needs an interface to Scarlett
     * @param Args... the argument types of the constructor
     * @see Curry
     */
    template <typename T, typename ...Args>
    T create(Args &&...args)
    {
        return T(std::forward<Args>(args)...);
    }
}
