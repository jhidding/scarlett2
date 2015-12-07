#pragma once
#include "../system/symbol.hh"
#include "../system/continuation.hh"
#include "../system/combiner.hh"

namespace Scarlett
{
    extern Continuation *f_lambda(Continuation *c, Ptr args);
    extern Continuation *f_case_lambda(Continuation *c, Ptr args);
    extern Continuation *f_define(Continuation *c, Ptr args);
    extern Continuation *f_callcc(Continuation *c, Ptr args);

    extern Continuation *f_cond(Continuation *c, Ptr args);
    extern Continuation *f_if(Continuation *c, Ptr args);

    extern Continuation *f_let(Continuation *c, Ptr args);
    extern Continuation *f_let_star(Continuation *c, Ptr args);
    extern Continuation *f_named_let(Continuation *c, Ptr args);

    extern Continuation *f_eval(Continuation *c, Ptr args);
    extern Continuation *f_eval_env(Continuation *c, Ptr args);
    extern Continuation *f_apply(Continuation *c, Ptr args);
}
