#pragma once
#include "atom.hh"
#include <complex>

namespace Scarlett
{
    using Complex = Atom<std::complex<double>>;
    
    inline Ptr make_polar(Ptr a_, Ptr b_)
    {
        double a = cast<Numeric>(a_)->get_double(),
               b = cast<Numeric>(b_)->get_double();
        return make_ptr<Complex>(std::polar(a, b));
    }
    
    inline Ptr make_rectangular(Ptr a_, Ptr b_)
    {
        double a = cast<Numeric>(a_)->get_double(),
               b = cast<Numeric>(b_)->get_double();
        return make_ptr<Complex>(std::complex<double>(a,b));
    }
}

