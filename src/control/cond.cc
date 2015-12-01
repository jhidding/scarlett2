#include "control.hh"

using namespace Scarlett;

Continuation *f_cond(Continuation *c, Ptr a)
{
    if (a == nullptr)
        return c->send(&undefined);

    Ptr pred = caar(a),
        expr = cdar(a),
        rest = cdr(a);

    Environment *env = c->environment();

    return link(
        eval(pred),
        lambda([expr, rest] (Continuation *c, Ptr a)
        {
            return (is_not_false(a) ? link(eval(expr), c)
                                    : return f_cond(c, rest));
        }), c);
}
