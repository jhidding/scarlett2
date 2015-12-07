#pragma once
#include "object.hh"
#include "tuple.hh"
#include "../interface/iter.hh"
#include <map>
#include <string>

namespace Scarlett
{
    /*! \brief Environment class
     *
     * A map with pointers to Object instances. If a name is not
     * found in the map, look-up deeper in hierarchy.
     */
    class Environment: public Object
    {
        Ptr parents;
        std::map<std::string, Ptr> table;

    public:
        Environment() {}

        /*! \brief Constructor */
        Environment(Ptr parents_):
            parents(parents_) {}

        /*! \brief Look-up method */
        Ptr look_up(std::string const &name) const {
            if (table.count(name) == 1)
                return table.at(name);

            for (Ptr p : iter(parents)) {
                try {
                    Ptr q = cast<Environment>(p)->look_up(name);
                    return q;
                } catch(...) {}
            }

            throw Exception(ERROR_lookup);
        }

        void bind(std::string const &name, Ptr value) {
            table[name] = value;
        }
    };

    /* We never handle an environment directly from the interpreter,
       Since we want to keep things const as much as possible.
       This object wraps a mutable environment pointer. */
    class CapturedEnvironment: public Object {
        Environment *env;

    public:
        CapturedEnvironment(Environment *env_):
            env(env_) {}

        Environment *get() const { return env; }
    };
}
