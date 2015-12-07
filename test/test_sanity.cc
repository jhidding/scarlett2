#include "../common/test/unittest.hh"
#include <functional>
#include <iostream>

namespace Sanity {
    typedef std::function<int (int)> F;

    F multiply(int a) {
        return [a] (int b) { return a * b; };
    }
}

Test::Unit _test_sanity("-", "test sanity of me.",
    [] () {
        std::cout << "Hello " << Sanity::multiply(6)(7) << std::endl;
        return true;
    });
