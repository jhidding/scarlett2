#include "../src/system/special.hh"
#include "../src/system/symbol.hh"
#include "../common/test/unittest.hh"
#include "../src/system/tuple.hh"
#include "../src/system/atom.hh"

using namespace Scarlett;

typedef Tuple<Ptr,Ptr,Ptr> Triple;

Test::Unit _test_tuple(
    "0002-tuple", "Checks basic functioning of Tuples.",
    [] () {
        Ptr a = "kees"_s,
            b = "flip"_s,
            c = "koek"_s,
            d = new Triple(a, b, c);

        assert(repr(d) == "#tuple(kees flip koek)");
        assert(symbol_to_string(std::get<0>(*cast<Triple>(d))) == "kees");
        return true;
    }
);

Test::Unit _test_list(
    "0003-list", "Checks basic functioning of linked lists.",
    [] () {
        assert(not nullptr);

        Ptr a = list(1_a, 2_a, 3_a, 4_a, "hoedje van"_a, "hoedje van"_a);
        assert(is_pair(a));
        assert(is_proper_list(a));

        Ptr b = list_reverse(a);
        assert(is_proper_list(b));
        assert(b->equal(list("hoedje van"_a, "hoedje van"_a, 4_a, 3_a, 2_a, 1_a)));

        assert(is_proper_list(list(1_a, 2_a, 3_a)));

        std::cout << repr(a) << std::endl;

        return true;
    }
);
