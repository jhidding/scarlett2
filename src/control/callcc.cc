#include "control.hh"
#include "link.hh"

using namespace Scarlett;

class CapturedContinuation: public Applicative {
    Continuation *c;

public:
    CapturedContinuation(Continuation *c_):
        c(c_) {}

    Continuation *operator()(Continuation *d, Ptr args) const {
        return c->send(args);
    }
};

Continuation *Scarlett::f_callcc(Continuation *c, Ptr args) {
    return (c << call(car(args)))->send(new CapturedContinuation(c));
}
