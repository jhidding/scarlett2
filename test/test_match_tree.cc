#include "../common/test/unittest.hh"

#include "../src/system/special.hh"
#include "../src/system/symbol.hh"
#include "../src/system/tuple.hh"
#include "../src/system/atom.hh"
#include "../src/reader/reader.hh"

#include "../src/algorithm/algorithm.hh"
#include <map>

using namespace Scarlett;

Test::Unit _test_match_tree(
    "0005-match-tree", "Test the match-tree algorithm that matches a "
    "parameter tree to an argument tree.",
    [] () {
        std::map<std::string, Ptr> should = {
            {"a", 1_a}, {"b", 2_a}, {"c", list(3_a, 4_a)}, {"d", list("elske"_a, 5_a, 6_a)} };
        std::map<std::string, Ptr> could;

        Ptr a = "(a (b c) . d)"_e,
            b = "(1 (2 (3 4)) \"elske\" 5 6)"_e;

        match_tree(a, b,
            [&could] (Symbol const *s, Ptr v) {
                could[symbol_to_string(s)] = v;
            });

        std::cout << "matching " << repr(a) << " with " << repr(b)
                  << " should give:\n";
        for (auto const &p : could) {
            std::cout << p.first << ": " << repr(p.second) << "\n";
            assert(p.second->equal(should[p.first]));
        }

        return true;
    });
