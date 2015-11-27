#pragma once
#include "object.hh"
#include "environment.hh"
#include "continuation.hh"

namespace Scarlett
{
    class Combiner: public Object
    {
    public:
        virtual Continuation *apply(Continuation *c, Ptr args) const = 0;
    };

    class Operative: public Combiner
    {
    public:
        virtual Applicative *wrap() const
            { return new WrappedOperative(this); }
    };

    class Applicative: public Combiner
    {
    public:
        virtual Operative *unwrap() const
            { return new UnwrappedApplicative(this); }
    };

    class WrappedOperative: public Applicative
    {
        Operative const *f;

    public:
        WrappedOperative(Operative const *f_): f(f_) {}
        virtual Operative const *unwrap() const
            { return f; }
        Continuation *apply(Continuation *c, Ptr args) const
            { return f->apply(c, args); }
    };

    class UnwrappedApplicative: public Operative
    {
        Applicative const *f;

    public:
        UnwrappedApplicative(Applicative const *f_): f(f_) {}
        virtual Applicative const *wrap() const
            { return f; }
        Continuation *apply(Continuation *c, Ptr args) const
            { return f->apply(c, args); }
    };

    inline is_combiner(Ptr p)
        { return is_type<Combiner>(p); }

    inline is_operative(Ptr p)
        { return is_type<Operative>(p); }

    inline is_applicative(Ptr p)
        { return is_type<Applicative>(p); }
}
