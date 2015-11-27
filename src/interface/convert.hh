#pragma once

#include "../system/object.hh"

namespace Scarlett
{
    /*! converts generic values to Scarlett pointers and back.
     *
     * When interfacing Scarlett with C++, we need to convert general
     * data to pointers. Data is encapsulated into \link Atom<T>.
     * However if type T is \link Ptr , nothing is done.
     */
    template <typename T>
    class Convert
    {
        public:
            //! converting from a \link ptr to type T
            static inline T const &from_ptr(ptr a)
                { return cast<Atom<T>>(a)->value(); }

            //! constructs an \link Atom<T> from a value of type T
            static inline Ptr to_ptr(T const &value)
                { return new Atom<T>(value); }
    };

    template <>
    class Convert<Ptr>
    {
        public:
            static inline Ptr from_ptr(ptr a)
                { return a; }
            static inline Ptr to_ptr(ptr a)
                { return a; }
    };
}
