#include "continuation.hh"
#include "../control/control.hh"
#include "../control/link.hh"

using namespace Scarlett;

class Root: public Continuation
{
    Ptr _result;

public:
    Root(Environment<> *environment_):
        Continuation(nullptr, environment_),
        _result(nullptr) {}

    virtual Ptr result() const
        { return _result; }

    virtual Continuation *send(Ptr value)
        { _result = value; return this; }

    virtual Continuation *step()
        { throw Exception(ERROR_runtime,
                          "Reached the root continuation."); }
};

Ptr Scarlett::run(Ptr program, Environment<> *env)
{
    auto root = new Root(env),
         cont = new Call<Function>(root, &f_eval);

    cont->send(program);

    while (cont != root)
        cont = cont->step();

    return root->result();
}
