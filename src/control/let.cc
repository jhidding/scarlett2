#include "control.hh"
#include "link.hh"
#include "../system/tuple.hh"
#include "closure.hh"

using namespace Scarlett;

Continuation *f_let(Continuation *c, Ptr a)
{
    if (is_symbol(car(a))) {
        return f_named_let(c, a);
    }

    Ptr args = map_reverse(car, car(a)),
        vals = map_reverse(cdr, car(a)),
        body = cadr(a);

    return c << seq_eval(body) << bind_to(args) << map_eval(vals);
}

Continuation *f_let_star(Continuation *c, Ptr a)
{
    Ptr args = map_reverse(car, car(a)),
        vals = map_reverse(cdr, car(a)),
        body = cadr(a);

    c = (c << seq_eval(body))->new_env();

    while (args)
    {
        c = c << bind_to(car(args)) << eval(car(vals));
        args = cdr(args);
        vals = cdr(vals);
    }

    return c;
}

Continuation *f_named_let(Continuation *c, Ptr a)
{
    Ptr name = car(a),
        args = map(car, cadr(a)),
        vals = map(cdr, cadr(a)),
        body = caddr(a);

    return ((c << call(name))->new_env() << map_eval(vals))
        ->bind(name, new ApplicativeClosure(c->environment(), args, body));
}
