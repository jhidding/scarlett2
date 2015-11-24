#include "../src/system/special.hh"
#include "../src/system/symbol.hh"
#include "../common/test/unittest.hh"
#include "../src/system/tuple.hh"

using namespace Scarlett;

Test::Unit _test_types(
    "0001-types", "Checks the validity of the fundamental `is_?()` functions.",
    [] () {
        Ptr a = new Special("ignore"),
            b = new Symbol("lambda"),
            c = new Pair(a, b);

        assert(is_special(a));
        assert(is_symbol(b));
        assert(!is_special(b));
        assert(!is_symbol(a));
        assert(is_pair(c));
        assert(!is_pair(b));

        return true;
    }
);
