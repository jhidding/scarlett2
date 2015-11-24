#include "control.hh"

using namespace Scarlett;

Cmd scrope_define(ptr<Environment> env, Ptr a)
{
    Ptr symbol_tree = car(a),
        expr        = cadr(b);
        
    return c_define(symbol_tree, expr);
}
