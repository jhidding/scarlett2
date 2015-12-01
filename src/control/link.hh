#pragma once
#include "../system/object.hh"
#include "../system/continuation.hh"
#include "../system/combiner.hh"

#include <utility>

namespace Scarlett
{
    typedef std::function<Continuation *(Continuation *)> Element;
    typedef std::function<Continuation *(Continuation *, Ptr)> Function;

    Continuation *link(Continuation *c)
        { return c; }

    template <typename ...Args>
    Continuation *link(Element const &f, Args &&...rest)
        { return f(link(std::forward<Args>(rest)...)); }

    Element eval(Ptr expr)
    {
        return [expr] (Continuation *c)
        {
            Continuation *d = new Apply(c, &f_eval);
            d->send(expr);
            return d;
        };
    }

    Element lambda(Function const &f)
    {
        return [f] (Continuation *c)
        {
            return new Apply(c, new C_Closure(f));
        };
    }
}
