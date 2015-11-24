#pragma once
#include <memory>
#include <functional>
#include "../../common/misc/utility.hh"
#include "../base/exception.hh"

#include <gc/gc_cpp.h>

namespace Scarlett
{
    using Misc::format;
    using string = std::string;

    class Object;
    class Type;

    template <typename T, typename ...Args>
    T *make_ptr(Args &&...args)
    { return new T(std::forward<Args>(args)...); }

    typedef Object const *Ptr;
    typedef Object *MutablePtr;

    inline bool is_mutable(Ptr) { return false; }
    inline bool is_mutable(MutablePtr) { return true; }

    class Object: public gc_cleanup
    {
    public:
        Object():
            gc_cleanup() {}

        virtual Type const *type() const;

        virtual ~Object() noexcept {}

        virtual string repr() const
            { return "#<not-implemented>"; }

        virtual bool eq(Ptr a) const
            { return a == this; }

        virtual bool equal(Ptr a) const
            { return eq(a); }
    };

    inline bool is_equal(Ptr a, Ptr b)
    {
        if (a == b) return true;
        if (a == nullptr or b == nullptr) return false;
        return a->equal(b);
    }

    class Type: public Object
    {
        string m_name;

    public:
        Type(string const &name_):
            m_name(name_) {}

        string const &name() const { return m_name; }
    };

    template <typename T>
    T const *cast(Ptr a)
        { return dynamic_cast<T const *>(a); }

    template <typename T>
    T *mutable_cast(MutablePtr m)
        { return dynamic_cast<T *>(m); }

    inline bool is_nil(Ptr a)
        { return a != nullptr; }

    template <typename T>
    bool is_type(Ptr a)
        { return dynamic_cast<T const *>(a) != nullptr; }

    extern Ptr const nil;
}
