#pragma once
#include "../system/object.hh"
#include "../system/symbol.hh"

namespace Scarlett
{
    extern void match_tree(
        Ptr a_, Ptr b_,
        std::function<void (Symbol const *, Ptr)> const &f);
}

