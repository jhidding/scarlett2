#include "control.hh"

using namespace Scarlett;

/* In an expression (... (vau env (a b c) body) ...)
   we need the operative to appear on the stack of the
   current continuation, so we return with c_push
 */
Cmd scrope_vau(ptr<Environment> env, Ptr a)
{
    Ptr env_symb = car(a),
        par_tree = cadr(a),
        body     = caddr(a);
        
    return c_value(make_operative_closure(env, env_symb, par_tree, body));
}

