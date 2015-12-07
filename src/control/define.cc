#include "control.hh"
#include "link.hh"

using namespace Scarlett;

Continuation *Scarlett::f_define(Continuation *c, Ptr args) {
    return c << bind_to(car(args)) << eval(cdr(args));
}
