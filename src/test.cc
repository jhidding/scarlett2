#include "system/tuple.hh"
#include "system/atom.hh"
#include "reader/reader.hh"

using namespace Scarlett;

template <unsigned ...i>
void print_seq(aux::seq<i...>)
{
    std::cout << format(i...);
}

int main()
{
    Ptr a = list(1_a, 2_a, 3_a, cons(42_a, 23_a, "haonth"_a));
    Ptr b = "1 2. 3.456 (42 . 1@1) \"Hello World!\" [a b c d]"_e; 
    std::cout << "hello ";
    std::cout << repr(a) << "\n";
    std::cout << repr(b) << "\n";
    
    while (b)
    {
        std::cout << car(b)->type_name() << " ";
        b = cdr(b);
    }
    std::cout << "\n";
    
    return 0;
}
