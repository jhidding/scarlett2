#include "control.hh"

using namespace Scarlett;

Cmd scrope_lambda(ptr<Environment> env, Ptr a)
{
    Ptr par_tree = car(a),
        body     = cadr(a);
        
    return c_value(make_applicative_closure(env, par_tree, body));
}

