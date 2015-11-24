#include "tuple.hh"
#include <stack>

using namespace Scarlett;

bool Scarlett::is_proper_list(Ptr a)
{
    while (a)
    {
        if (not is_pair(a))
            return false;
        a = cdr(a);
    }

    return true;
}

std::string Scarlett::proper_list_repr(Ptr a)
{
    std::string r = "(";
    while (a)
    {
        r += repr(car(a));
        a = cdr(a);
        r += (a ? " " : ")");
    }
    return r;
}

std::string Scarlett::repr(Ptr a)
{
    if (a)
        if (is_proper_list(a))
            return proper_list_repr(a);
        else
            return a->repr();
    else
        return "()";
}

Ptr Scarlett::list_reverse(Ptr p)
{
    Ptr s = nil;

    while (p)
    {
        s = cons(car(p), s);
        p = cdr(p);
    }

    return s;
}

bool Scarlett::pair_equal(Ptr a_, Ptr b_)
{
    std::stack<Ptr> sa, sb;
    sa.push(a_); sb.push(b_);

    while (not sa.empty())
    {
        if (sb.empty()) return false;

        Ptr a = sa.top(), b = sb.top();
        sa.pop(); sb.pop();

        if (is_pair(a))
        {
            if (not is_pair(b)) return false;

            // we go depth first
            sa.push(cdr(a)); sb.push(cdr(b));
            sa.push(car(a)); sb.push(car(b));

            continue;
        }

        if (not is_equal(a, b)) return false;
    }

    if (not sb.empty()) return false;

    return true;
}
