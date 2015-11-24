#include "continuation.hh"

using namespace Scarlett;

Cmd scrot_eval(ptr<Environment> env, Ptr expr)
{
    if (is_symbol(expr))
    {
        return c_value(environment->look_up(expr));
    }
       
    if (is_pair(expr))
    {
        Ptr comb = car(expr), args = cdr(expr);
        
        if (is_symbol(comb))
            comb = environment->look_up(expr);
        
        if (is_applicative(comb))
            return c_exec(env,
                c_eval_list(args),
                c_apply(comb));
        
        if (is_operative(comb))
            return cast<Operative>(comb)->apply(env, args);
            
        if (is_pair(comb))
            return c_exec(env,
                c_set_stack(args),
                c_eval(comb),
                c_apply_operative(env, s_eval));
    }
    
    return c_value(expr);    
}

Cmd Continuation::eval(C_Task const &task) const
{
    switch (task.id())
    {
        case C_PASS:
            return task.cmd();
            
        case C_EVAL:
            return scrot_eval(env, task.expr());
        
        case C_APPLY:
            return c_parent(task.apply(S));
    } 
}

ptr<Context> Continuation::handle(ptr<Cmd> rv)
{   
    switch (rv.id())
    {
        case C_CHILD:
            return rv->make_child(self());
                
        case C_SIBLING:
            return rv->make_child(parent());
                    
        case C_PARENT: // could this cause a cascade of handle calls?
            return parent()->handle(rv->value());
                                                
        case C_PUSH:
            S = cons(rv->value(), S);
            return self();
            
        case C_SET:
            S = rv->value();
            return self();
            
        case C_DEFINE:
            environment->bind(
                cast<Symbol>(car(Cmd.second)),
                cadr(Cmd.second));
            return parent();
            
        case C_THIS:
            S = cons(self(), S);
            return self();
        
        case C_ENVIRONMENT:
            S = cons(environment, S);
            return self();
            
        default:
            throw Exception(ERROR, "Unknown instruction in continuation byte code");
    }
}

