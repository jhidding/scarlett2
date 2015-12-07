#include "control.hh"

using namespace Scarlett;

class Conditional: public Continuation {
    Continuation *a, *b;
public:
    Conditional(Continuation *c, Continuation *a_, Continuation *b_):
        parent(c), a(a_), b(b_) {}

    Ptr result() { return &undefined; }

    Continuation *send(Ptr p) {
        return (is_not_false(p) ? a : b);
    }

    Continuation *step() {
        throw Exception(ERROR_runtime,
            "Conditional::step() should not be called.");
    }
};

Element cond(Element a, Element b) {
    return [a, b] (Continuation *c) {
        return new Conditional(a(c), b(c));
    };
}

Continuation *f_cond(Continuation *c, Ptr a)
{
    if (a == nullptr)
        return c->send(&undefined);

    Ptr pred = caar(a),
        expr = cdar(a),
        rest = cdr(a);

    return c << cond(eval(expr), apply(&f_cond, rest)) << eval(pred);
}

Continuation *f_if(Continuation *c, Ptr a)
{
    Ptr pred = car(a),
        opt1 = cadr(a),
        opt2 = caddr(a);

    return c << cond(eval(opt1), eval(opt2)) << eval(pred);
}
