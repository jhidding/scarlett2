#pragma once
#include "object.hh"
#include "environment.hh"
#include "continuation.hh"

namespace Scarlett
{
    class Closure: public Operative
    {
        Environment     *env;
        Ptr              nle_symb;
        Ptr              pars;
        Ptr              body;

    public:
        Closure(Environment *env_, Ptr nl_, Ptr pars_, Ptr body_):
            env(env_), nle_symb(nl_), pars(pars_), body(body_) {}

        Continuation *apply(Continuation *c, Ptr args) const
        {
            auto new_env = new Environment(
                env, pars, args));

            Environment *nl_env = c->environment();
            new_env->bind(nle_symb, nl_env);

            return link(seq_eval(new_env, body), c);
        }
    };

    Ptr make_operative_closure(Environment *env, Ptr nl, Ptr par_tree, Ptr body)
    {
        return new OperativeClosure(env, nl, par_tree, body);
    }

    class ApplicativeClosure: public Applicative
    {
        Environment     *env;
        Ptr              pars;
        Ptr              body;

    public:
        Closure(Environment *env_, Ptr pars_, Ptr body_):
            env(env_), pars(pars_), body(body_) {}

        Continuation *apply(Continuation *c, Ptr args) const
        {
            auto new_env = new Environment(
                env, match_tree(pars, args));

            return link(seq_eval(new_env, body), c);
        }
    };

    Ptr make_applicative_closure(Environment *env, Ptr par_tree, Ptr body)
    {
        return new ApplicativeClosure(env, par_tree, body);
    }

    class C_Closure: public Applicative
    {
//        Ptr capture;
        std::function<Continuation *(Continuation *,Ptr)> f;

    public:
        template <typename T>
        C_Closure(T const &f_):
            f(f_) {}

        Continuation *apply(Continuation *c, Ptr p) const
        {
            return f(c, p);
        }
    };

    template <typename T>
    Ptr make_c_closure(T const &func)
    {
        return new C_Closure(func));
    }
}
