#pragma once
#include "../system/combiner.hh"
#include "../system/environment.hh"
#include "control.hh"
#include "link.hh"

namespace Scarlett
{
    class OperativeClosure: public Operative {
        Environment     *env;
        Ptr              nle_symb;
        Ptr              pars;
        Ptr              body;

    public:
        OperativeClosure(Environment *env_, Ptr nl_, Ptr pars_, Ptr body_):
            env(env_), nle_symb(nl_), pars(pars_), body(body_) {}

        Continuation *operator()(Continuation *c, Ptr args) const {
            return (c << seq_eval(body))
                ->set_env(env)
                ->new_env()
                ->bind(pars, args)
                ->bind(nle_symb, c->environment());
        }
    };

    class ApplicativeClosure: public Applicative {
        Environment     *env;
        Ptr              pars;
        Ptr              body;

    public:
        ApplicativeClosure(Environment *env_, Ptr pars_, Ptr body_):
            env(env_), pars(pars_), body(body_) {}

        Continuation *operator()(Continuation *c, Ptr args) const {
            return (c << seq_eval(body))
                ->set_env(env)
                ->new_env()
                ->bind(pars, args);
        }
    };

    class CaseLambda: public Applicative {
        Environment *env;
        Ptr lambdas;

    public:
        CaseLambda(Environment *env_, Ptr lambdas_):
            env(env_), lambdas(lambdas_) {}

        Continuation *operator()(Continuation *c, Ptr args) const {
            Ptr opts = lambdas;

            while (opts) {
                Ptr pars = caar(opts),
                    body = cdar(opts);

                Continuation *d = (c << seq_eval(body))
                    ->set_env(env)
                    ->new_env();

                try {
                    return d->bind(pars, args);
                } catch (...) {}

                opts = cdr(opts);
            }

            throw Exception(ERROR_match,
                "Arguments do not match any of the cases in case-lambda.");
        }
    };
}
