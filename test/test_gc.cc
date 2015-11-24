#include "../src/system/object.hh"
#include "../common/test/unittest.hh"

using namespace Scarlett;

class A: public Object
{
    int &v;

public:
    A(int &v_):
        Object(), v(v_) {}

    virtual ~A() noexcept
    {
        std::cout << "destructing A\n";
        v = 42;
    }
};

class B: public Object
{
    A *a;

public:
    B(int &v_):
        Object(), a(new A(v_)) {}

    virtual ~B() noexcept
    {
        std::cout << "destructing B\n";
    }
};

Test::Unit _test_gc(
    "0000-gc", "Test the basic functioning of the "
    "garbage collector. This test only works if compiled "
    "without optimisation.",
    [] () {
        int v = 0;
        auto b = new B(v);

        delete b;
        GC_gcollect();

        if (v != 42) return false;

        return true;
    });
