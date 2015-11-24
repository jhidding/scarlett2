#pragma once
#include "object.hh"
#include <vector>

namespace Scarlett
{
    class Vector: public Object, public std::vector<Ptr>
    {
    public:
        Vector() {}

        virtual std::string repr() const;
        virtual bool equal(Ptr a_) const;
    };
}
