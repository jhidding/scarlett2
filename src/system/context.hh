#pragma once
#include "object.hh"
#include "tuple.hh"
#include <queue>

namespace Scarlett
{
    /*! \brief Abstact base class for Context objects
     *
     * The Context class is an abstract base class for a stack based
     * evaluation design pattern. This pattern pops op everywhere, so
     * we want some good fundamental code to deal with these cases.
     *
     * For every use case, we would define a little language that is
     * understood by the evaluator. This then returns a list of instructions
     * for the context, manipulating the stack and general execution flow.
     * This list of instructions is passed through the member 'handle',
     * which returns a pointer to the context on which we continue the
     * evaluation loop.
     *
     * Eventually I'd like to be able to take this into such a shape
     * that any desired state machine will run on the main evaluation
     * loop.
     */
    template <typename T, typename CLst>
    class Context: public Object
    {
        Context *m_parent;

    public:
        typedef T Task;

        Context(Context *parent_):
            Object(), m_parent(parent_) {}

        Context *parent() const
            { return m_parent; }

        virtual bool empty() const = 0;
        virtual T const &top() const = 0;
        virtual void pop() = 0;

        virtual CLst eval(Task const &t) const = 0;
        virtual Context *handle(CLst &return_value) = 0;
        virtual Ptr result() const = 0;
    };

    template <typename Cx, typename ...Args>
    Ptr run_context(Args &&...args)
    {
        typedef typename Cx::Task Task;

        Cx *c = new Cx(nullptr, std::forward<Args>(args)...);
        //c = c->handle(init());

        while (c and not c->empty())
        {
            Task const &t = c->top();
            auto t_result = c->eval(t);

            c->pop();
            c = dynamic_cast<Cx *>(c->handle(t_result));
        }

        return c->result();
    }
}
