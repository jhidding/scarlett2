#include "../src/system/special.hh"
#include "../src/system/symbol.hh"
#include "../common/test/unittest.hh"
#include "../src/system/tuple.hh"
#include "../src/system/atom.hh"
#include "../src/reader/reader.hh"

using namespace Scarlett;

Test::Unit _test_read(
    "0004-read", "Reads scheme expressions.",
    [] () {
        Ptr a = "1 2 3 4 \"hoedje van\" \"hoedje van\""_e;
        assert(is_pair(a));
        assert(is_proper_list(a));

        Ptr b = "1 2. 3.456 (42 . 1@1) \"Hello World!\" [a b c d]"_e;

        std::cout << repr(a) << std::endl << repr(b) << std::endl;
        
        return true;
    }
);
