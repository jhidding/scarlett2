#include "../control/control.hh"
#include "libraries.hh"

using namespace Scarlett;

Global<Library> lib_scheme_base("(scheme base)", nullptr,
    [] (Library::Binder bind) {
        bind("eval", c_operative(&f_eval_env));
        bind("apply", c_applicative(&f_apply));
        bind("define", c_operative(&f_define));
        bind("if", c_operative(&f_if));
        bind("cond", c_operative(&f_cond));
        bind("call/cc", c_applicative(&f_callcc));
        bind("case-lambda", c_operative(&f_case_lambda));
        bind("let", c_operative(&f_let));
        bind("let*", c_operative(&f_let_star));
    });
