#pragma once

namespace Scarlett
{
    /*! represents part of the computation, knows where results should go.
     */
    class Node: public Object
    {
        Ptr _parents;   // list of parent continuations

    public:
        Node(Ptr parents_):
            _parents(parents_) {}

        virtual Continuation *
    };

    /*! is called by a function to return results to the correct place
     */
    class Continuation: public Object
    {
        Node        *n;
        Environment *env;
        unsigned    address;

    public:
        virtual Continuation *send(Ptr value) const = 0;
    };
}
