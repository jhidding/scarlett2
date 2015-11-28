#pragma once
#include "object.hh"
#include "numeric.hh"
//#include "../../common/misc/utility.hh"
#include <tuple>
#include <stack>

namespace Scarlett
{
    extern bool is_proper_list(Ptr a);
    extern std::string proper_list_repr(Ptr a);
    extern std::string repr(Ptr a);

    namespace aux {
        inline std::string intersperse_string(
            std::string const &sep,
            Ptr last)
        {
            return repr(last);
        }

        template <typename ...Args>
        std::string intersperse_string(
            std::string const &sep,
            Ptr first,
            Args &&...rest)
        {
            return repr(first) + sep + intersperse_string(sep,
                    std::forward<Args>(rest)...);
        }

        template <unsigned ...n>
        struct seq {};

        template <unsigned n, unsigned ...i>
        struct make_seq : make_seq<n-1, n-1, i...> {};

        template <unsigned ...i>
        struct make_seq<0, i...> : seq<i...> {};

        template <typename T, unsigned ...i>
        std::string tuple_string(T const &tuple, seq<i...>)
        {
            return intersperse_string(" ", std::get<i>(tuple)...);
        }
    }

    class TupleBase: public Object
    {
    public:
        virtual size_t size() const = 0;
    };

    extern bool pair_equal(Ptr a_, Ptr b_);

    template <typename ...Args>
    class Tuple: public TupleBase, public std::tuple<Args...>
    {
    public:
        using std::tuple<Args...>::tuple;

        size_t size() const
            { return sizeof...(Args); }

        std::string repr() const;

        bool equal(Ptr b) const
        {
            if (is_pair(this))
                return pair_equal(this, b);
            else
                return eq(b);
        }
    };

    template <typename ...Args>
    std::string Tuple<Args...>::repr() const
    {
        return "#tuple(" + aux::tuple_string(*this, aux::make_seq<sizeof...(Args)>()) + ")";
    }

    typedef Tuple<Ptr,Ptr> Pair;

    template <typename ...Args>
    inline Ptr cons(Args ...args)
        { return Ptr(new Tuple<Args...>(args...)); }

    inline bool is_pair(Ptr a)
        { return is_type<Pair>(a); }

    inline Ptr list()
        { return nullptr; }

    extern Ptr list_reverse(Ptr);

    template <typename ...Args>
    Ptr list(Ptr a, Args &&...args)
        { return cons(a, list(std::forward<Args>(args)...)); }


/*    template <typename T, typename ...Args>
    Ptr list(T a, Args &&...args)
        { return cons(new Atom<T>(a), list(std::forward<Args>(args)...)); }
*/
    inline Ptr car(Ptr a) { return std::get<0>(*cast<Pair>(a)); }
    inline Ptr cdr(Ptr a) { return std::get<1>(*cast<Pair>(a)); }

    inline Ptr caar(Ptr p) { return car(car(p)); }
    inline Ptr cdar(Ptr p) { return cdr(car(p)); }
    inline Ptr cadr(Ptr p) { return car(cdr(p)); }
    inline Ptr cddr(Ptr p) { return cdr(cdr(p)); }

    inline Ptr caaar(Ptr p) { return car(caar(p)); }
    inline Ptr cdaar(Ptr p) { return cdr(caar(p)); }
    inline Ptr cadar(Ptr p) { return car(cdar(p)); }
    inline Ptr cddar(Ptr p) { return cdr(cdar(p)); }
    inline Ptr caadr(Ptr p) { return car(cadr(p)); }
    inline Ptr cdadr(Ptr p) { return cdr(cadr(p)); }
    inline Ptr caddr(Ptr p) { return car(cddr(p)); }
    inline Ptr cdddr(Ptr p) { return cdr(cddr(p)); }

    inline Ptr caaaar(Ptr p) { return car(caaar(p)); }
    inline Ptr cdaaar(Ptr p) { return cdr(caaar(p)); }
    inline Ptr cadaar(Ptr p) { return car(cdaar(p)); }
    inline Ptr cddaar(Ptr p) { return cdr(cdaar(p)); }
    inline Ptr caadar(Ptr p) { return car(cadar(p)); }
    inline Ptr cdadar(Ptr p) { return cdr(cadar(p)); }
    inline Ptr caddar(Ptr p) { return car(cddar(p)); }
    inline Ptr cdddar(Ptr p) { return cdr(cddar(p)); }
    inline Ptr caaadr(Ptr p) { return car(caadr(p)); }
    inline Ptr cdaadr(Ptr p) { return cdr(caadr(p)); }
    inline Ptr cadadr(Ptr p) { return car(cdadr(p)); }
    inline Ptr cddadr(Ptr p) { return cdr(cdadr(p)); }
    inline Ptr caaddr(Ptr p) { return car(caddr(p)); }
    inline Ptr cdaddr(Ptr p) { return cdr(caddr(p)); }
    inline Ptr cadddr(Ptr p) { return car(cdddr(p)); }
    inline Ptr cddddr(Ptr p) { return cdr(cdddr(p)); }
}
