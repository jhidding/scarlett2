#pragma once

inline void for_each(
    Object::GC const &f, int i,
    Ptr last)
{
    f(last, format(i));
}

template <typename ...Args>
void for_each(
    Object::GC const &f, int i,
    Ptr first,
    Args &&...rest)
{
    f(first, format(i));
    for_each(f, i+1, std::forward<Args>(rest)...);
}

template <typename T, unsigned ...i>
void tuple_for_each(Object::GC const &f,
    T const &tuple, seq<i...>)
{
    for_each(f, 0, std::get<i>(tuple)...);
}

template <typename ...Args>
void tuple_for_each(Object::GC const &f,
    std::tuple<Args...> const &tuple)
{
    tuple_for_each(f, tuple, make_seq<sizeof...(Args)>());
}
