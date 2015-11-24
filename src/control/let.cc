#include "control.hh"

using namespace Scarlett;

/* bit of wishful programming here
   (let ((a make-a-expr)
         ((b c) make-bc-expr))
     body)
     
  translates to
   (apply (lambda  (a (b c)) body)
          (list make-a-expr make-bc-expr))
 */

/* using:
    c_eval to have a list of expressions evaluated
    c_apply the stack list to a function
    make_closure to do Ptr(new Closure(...))
 */
Cmd scrope_let(ptr<Environment> env, Ptr a)
{
    Ptr defines  = car(a),
        body     = cadr(a);
  
    return c_eval(env, cons(
        make_closure(env, list_map(car, defines), body),
        list_map(cadr, defines)));
}

/* let* can be implemented as a series of nested let */
Cmd scrope_let_star(ptr<Environment> env, Ptr a)
{
    Ptr defines = car(a),
        body    = cadr(a);

    ptr<Environment> new_env(new Environment(list(env)));
        
    return 
}

/*
    return c_exec(new_env,
        c_push(list_map(car, defines)),
        c_eval(new_env, list_map(cdr, defines)),
        c_apply(scm_define),
        c_eval_tail(new_env, body));
*/
