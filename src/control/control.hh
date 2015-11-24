#pragma once
#include "../system/continuation.hh"
#include "../system/combiner.hh"

namespace Scarlett
{
    extern Cmd scrope_cond(ptr<Environment> env, Ptr args);
    extern Cmd scrope_if(ptr<Environment> env, Ptr args);
    extern Cmd scrope_begin(ptr<Environment> env, Ptr args);
    extern Cmd scrope_eval(ptr<Environment> env, Ptr args);
}

