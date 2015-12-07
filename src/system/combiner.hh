#pragma once
#include "object.hh"
#include "environment.hh"
#include "continuation.hh"

namespace Scarlett
{
    typedef Continuation *(*Function)(Continuation *, Ptr);

    class Combiner: public Object
    {
    public:
        virtual Continuation *operator()(Continuation *c, Ptr args) const = 0;
    };

    class Operative: public Combiner
    {};

    class Applicative: public Combiner
    {};

    inline bool is_combiner(Ptr p)
        { return is_type<Combiner>(p); }

    inline bool is_operative(Ptr p)
        { return is_type<Operative>(p); }

    inline bool is_applicative(Ptr p)
        { return is_type<Applicative>(p); }

    class C_Operative: public Operative {
        Function f;
    public:
        C_Operative(Function f_): f(f_) {}
        virtual Continuation *operator()(Continuation *c, Ptr args) const {
            return (*f)(c, args);
        }
    };

    inline C_Operative *c_operative(Function f) {
        return new C_Operative(f); }

    class C_Applicative: public Applicative {
        Function f;
    public:
        C_Applicative(Function f_): f(f_) {}
        virtual Continuation *operator()(Continuation *c, Ptr args) const {
            return (*f)(c, args);
        }
    };
    
    inline C_Applicative *c_applicative(Function f) {
        return new C_Applicative(f); }
}
