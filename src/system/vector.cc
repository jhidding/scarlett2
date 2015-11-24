#include "vector.hh"

using namespace Scarlett;

std::string Vector::repr() const
{
    std::string s = "#(";

    if (size() > 0)
        s += at(0)->repr();

    for (size_t i = 1; i < size(); ++i)
        s += " " + at(i)->repr();

    s += ")";

    return s;
}

bool Vector::equal(Ptr a_) const
{
    Vector const *a = cast<Vector>(a_);
    if (size() != a->size()) return false;

    for (size_t i = 0; i < size(); ++i)
        if (not is_equal(at(i), a->at(i)))
            return false;

    return true;
}
