#pragma once
#include "object.hh"
#include "environment.hh"
#include "continuation.hh"

namespace Scarlett
{
    class Combiner: public Object
    {
    public:
        virtual Cmd apply(Ptr args) const = 0;
    };

    class Operative: public Combiner
    {
    public:
    };

    class Applicative: public Combiner
    {
    public:
    };
}
