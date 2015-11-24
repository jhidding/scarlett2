#include "control.hh"

using namespace Scarlett;

Cmd scrope_if(ptr<Environment> env, Ptr a)
{
    Ptr pred = car(a),
        opt1 = cadr(a),
        opt2 = caddr(a);
        
    return c_exec(env, 
        c_eval(pred),
        c_apply(c_applicative([env, opt1, opt2] (Ptr args)
        {
            if (args)
                return c_eval_tail(env, opt1);
            else
                return c_eval_tail(env, opt2);
        })));
}
