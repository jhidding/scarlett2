#pragma once
#include "object.hh"
#include "tuple.hh"
#include "string_utility.hh"
#include <map>
#include <string>

namespace Scarlett
{
    /*! \brief Environment class
     *
     * A map with pointers to Object instances. If a name is not
     * found in the map, look-up deeper in hierarchy.
     */
    template <typename Key = string, typename Value = Object const>
    class Environment: public Object
    {
        Ptr parents;
        std::map<Key, Value *> table;
        
    public:
        Environment() {}

        /*! \brief Constructor */
        Environment(
                Ptr parents_,
                std::map<Key, Value *> const &table_):
            parents(parents_),
            table(table_)
        {}

        /*! \brief Look-up method */
        Value *look_up(Key const &name) const
        {
            if (table.count(name) == 1)
                return table.at(name);

            Ptr *p = parents;
            while (is_pair(p))
            {
                Value *q = cast<Environment>(car(p))->look_up(name);
                if (not is_null(q))
                    return q;
                p = cdr(p);
            }

            return nil;
        }
    };
}
