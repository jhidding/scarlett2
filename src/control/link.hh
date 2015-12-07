#pragma once
#include "../system/object.hh"
#include "../system/continuation.hh"
#include "../system/combiner.hh"
#include "control.hh"

#include <utility>

namespace Scarlett
{
    typedef std::function<Continuation *(Continuation *)> Element;

    inline Continuation *operator<<(Continuation *c, Element const &e) {
        return e(c);
    }

    template <typename T>
    class Call: public Continuation {
        T func;

    public:
        Call(Continuation *parent_, T func_):
            Continuation(parent_),
            func(func_) {}

        Continuation *step() {
            return (*func)(parent(), result());
        }
    };

    class CallByName: public Continuation {
        Ptr name;

    public:
        CallByName(Continuation *parent_, Ptr name_):
            Continuation(parent_),
            name(name_) {}

        Continuation *step() {
            return (*cast<Combiner>(look_up(name)))(
                parent(), result());
        }
    };

    Element call(Function func) {
        return [func] (Continuation *c) -> Continuation * {
            return new Call<Function>(c, func);
        };
    }

    Element call(Ptr func) {
        if (is_symbol(func)) {
            return [func] (Continuation *c) -> Continuation * {
                return new CallByName(c, func);
            };
        }

        if (is_combiner(func)) {
            return [func] (Continuation *c) -> Continuation * {
                return new Call<Combiner const *>(c, cast<Combiner>(func));
            };
        }

        throw Exception(ERROR_runtime, "Attempting call to non-callable.");
    }

    Element eval(Ptr expr) {
        return [expr] (Continuation *c) -> Continuation * {
            return (c << call(&f_eval))->send(expr);
        };
    }

    class Binder: public Continuation {
        Ptr name;

    public:
        Binder(Continuation *parent_, Ptr name_):
            Continuation(parent_),
            name(name_) {}

        Continuation *send(Ptr value) {
            return parent()->bind(name, value);
        }

        Continuation *step() {
            throw Exception(ERROR_runtime,
                "Binder::step() should not be called");
        }
    };

    Element bind_to(Ptr name) {
        return [name] (Continuation *c) {
            return new Binder(c, name);
        };
    }

    class Sequence: public Continuation {
        Ptr body;

    public:
        Sequence(Continuation *parent_, Ptr body_):
            Continuation(parent_), body(body_) {}

        Continuation *send(Ptr value) {
            return this;
        }

        Continuation *step() {
            Ptr expr = car(body);
            body = cdr(body);

            if (body) {
                return this << eval(expr);
            } else {
                return parent() << eval(expr);
            }
        }
    };

    Element seq_eval(Ptr body) {
        return [body] (Continuation *c) {
            return new Sequence(c, body);
        };
    }

    template <typename F>
    class Mapper: public Continuation {
        F f;
        Ptr src, tgt;

    public:
        Mapper(Continuation *parent_, F f_, Ptr src_):
            Continuation(parent_), f(f_), src(src_), tgt(nullptr) {}

        Ptr result() const {
            return reverse(tgt);
        }

        Continuation *send(Ptr value) {
            tgt = cons(value, tgt);
            return this;
        }

        Continuation *step() {
            if (src) {
                Ptr args = list(car(src));
                src = cdr(src);
                return (this << call(f))->send(args);
            } else {
                return parent()->send(result());
            }
        }
    };

    Element map_eval(Ptr src) {
        return [src] (Continuation *c) -> Continuation * {
            return new Mapper<Function>(c, &f_eval, src);
        };
    }
}
