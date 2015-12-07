#include "../common/test/unittest.hh"
#include "../src/system/atom.hh"
#include "../src/reader/reader.hh"
#include "../src/system/environment.hh"

using namespace Scarlett;

Test::Unit _test_environment("0007-environment", "Test the working of the"
    " environment class.",
    [] () {
        auto e = new Environment(),
             f = new Environment(list(e));

        Ptr p = 5_a, q = 6_a;

        e->bind("a", p);
        assert(e->look_up("a") == p);

        f->bind("b", q);
        assert(f->look_up("b") == q);
        assert(f->look_up("a") == p);

        try {
            f->look_up("c");
            throw Exception(ERROR_lookup,
                "'look_up' should have thrown error.");
        } catch (...) {}

        return true;
    });
