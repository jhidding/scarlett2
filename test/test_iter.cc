#include "../src/interface/iter.hh"
#include "../common/test/unittest.hh"
#include <iostream>

using namespace Scarlett;

Test::Unit _test_iter(
    "0006-iter", "Iterate over a list in a C++ fashion",
    [] () {
        Ptr a = list(1_a, 2_a, 3_a, 4_a);
        for (Ptr i : iter(a))
            std::cout << repr(i) << " ";
        std::cout << std::endl;
        return true;
    });
