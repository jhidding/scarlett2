/*  This file is part of Scarlett.

    Scarlett is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Scarlett is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Scarlett.  If not, see <http://www.gnu.org/licenses/>. */

#pragma once
#include "object.hh"
#include "symbol.hh"
#include "tuple.hh"
#include "environment.hh"
#include "../algorithm/algorithm.hh"

namespace Scarlett
{
    class Continuation: public Object {
        Continuation   *_parent;
        Environment  *_environment;
        Ptr             _result;

    public:
        Continuation(Continuation *parent_):
            _parent(parent_),
            _environment(parent_->environment()) {}

        Continuation(Continuation *parent_, Environment *env_):
            _parent(parent_),
            _environment(env_) {}

        Environment *environment() const {
            return _environment;
        }

        Continuation *parent() const {
            return _parent;
        }

        Continuation *new_env() {
            _environment = new Environment(list(_environment));
            return this;
        }

        Continuation *set_env(Environment *env) {
            _environment = env;
            return this;
        }

        Continuation *bind(Ptr name, Ptr value) {
            match_tree(name, value,
                [this] (Symbol const *s, Ptr value) {
                    _environment->bind(s->name(), value);
                });
            return this;
        }

        Ptr look_up(Ptr name) {
            return _environment->look_up(cast<Symbol>(name)->name());
        }

        virtual Ptr result() const {
            return _result;
        }

        virtual Continuation *send(Ptr value) {
            _result = value; return this;
        }

        virtual Continuation *step() = 0;
    };

    inline bool is_continuation(Ptr p) {
        return is_type<Continuation>(p);
    }
    
    /*! runs a program in a given environment.
     *
     * Creates a Root continuation, and one Apply continuation
     * evaluating the program. When the evaluation cascades back
     * to the root, return the result.
     */
    extern Ptr run(Ptr program, Environment *env);
}
