#pragma once
#include "../system/tuple.hh"
#include <iterator>

namespace Scarlett
{
    class ListIterator: public std::iterator<std::forward_iterator_tag, Ptr>
    {
        Ptr a;

    public:
        ListIterator() = default;
        ListIterator(Ptr a_): a(a_) {}

        ListIterator &operator++()
            { if (is_pair(a)) { a = cdr(a); return *this; }
              else throw Exception(ERROR_type, "type error in iterating a list, expecting a pair."); }

        bool operator==(ListIterator &i) const
            { return a == i.a; }

        bool operator!=(ListIterator &i) const
            { return a != i.a; }

        Ptr const operator*() const
            { if (is_pair(a)) { return car(a); }
              else throw Exception(ERROR_type, "type error in dereferencing list item, expecting a pair."); }
    };

    class ListRange
    {
        Ptr a;

    public:
        ListRange(Ptr a_): a(a_) {}

        ListIterator begin() { return ListIterator(a); }
        ListIterator end() { return ListIterator(nil); }
    };

    ListRange iter(Ptr a) { return ListRange(a); }
}
