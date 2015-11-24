#include "control.hh"

using namespace Scarlett;

Cmd scrope_cond(ptr<Environment> env, Ptr a)
{
    if (is_nil(a))
        return c_value(undefined);
    
    Ptr pred = caar(a),
        expr = cdar(a),
        rest = cdr(a);
    
    return c_exec(env,
        c_eval(pred),
        c_apply(c_applicative([env, expr, rest] (Ptr args)
        {
            if (args)
                return c_eval_seq(env, expr);
            else
                return scrope_cond(env, rest);
        })));
}
