#pragma once
#include <string>
#include <iostream>
#include <sstream>

namespace Misc
{
    inline void format_to(std::ostream &out) {}

    template <typename First, typename ...Rest>
    void format_to(std::ostream &out, First a, Rest &&...rest)
    {
        out << a;
        format_to(out, std::forward<Rest>(rest)...);
    }

    template <typename ...Args>
    std::string format(Args &&...args)
    {
        std::ostringstream ss;
        format_to(ss, std::forward<Args>(args)...);
        return ss.str();
    }
    
    template <typename T>
    class Reversed
    {
        T const &p;
    public:
        using const_iterator = typename T::const_reverse_iterator;
        using iterator = typename T::reverse_iterator;
        
        Reversed(T const &p_): p(p_) {}
        const_iterator begin() const { return p.rbegin(); }
        const_iterator end() const { return p.rend(); }  
    };
    
    template <typename T>
    Reversed<T> reversed(T const &p) { return Reversed<T>(p); }
}
