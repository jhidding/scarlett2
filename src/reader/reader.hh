#pragma once
#include "../system/object.hh"
#include <iostream>
#include <sstream>

namespace Scarlett
{ 
    extern Ptr read(std::string const &s);
    
    inline Ptr operator"" _e(char const *a, size_t s)
    {
        std::string str(a);
        return read(str);
    }
}

