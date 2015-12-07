#include "control.hh"
#include "closure.hh"

using namespace Scarlett;

Continuation *Scarlett::f_lambda(Continuation *c, Ptr args) {
    Ptr pars = car(args),
        body = cadr(args);

    return c->send(new ApplicativeClosure(c->environment(), pars, body));
}

Continuation *Scarlett::f_case_lambda(Continuation *c, Ptr args) {
    return c->send(new CaseLambda(c->environment(), args));
}
