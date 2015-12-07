#include "control.hh"
#include "link.hh"
#include "../system/object.hh"

using namespace Scarlett;

Continuation *Scarlett::f_apply(Continuation *c, Ptr args) {
    Combiner const *f = cast<Combiner>(car(args));
    return (*f)(c, cdr(args));
}

Continuation *f_eval_env(Continuation *c, Ptr args) {
    Ptr env = car(args),
        body = cadr(args);

    return (c << eval(body))->set_env(cast<CapturedEnvironment>(env)->get());
}

Continuation *f_eval(Continuation *c, Ptr expr) {
    if (is_symbol(expr)) {
        return c->send(c->look_up(expr));
    }

    if (is_pair(expr)) {
        Ptr comb = car(expr),
            args = cdr(expr);

        if (is_pair(comb)) {
            return c << call(&f_apply) << map_eval(expr);
        }

        if (is_symbol(comb)) {
            comb = c->look_up(comb);
        }

        if (is_applicative(comb)) {
            return c << call(comb) << map_eval(args);
        }

        if (is_operative(comb)) {
            return (c << call(comb))->send(args);
        }

        throw Exception(ERROR_runtime,
            "Expected operator, got ", repr(comb), ".");
    }

    return c->send(expr);
}
