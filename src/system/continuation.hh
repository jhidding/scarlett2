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
#include "environment.hh"

namespace Scarlett
{
    class Continuation: public Object
    {
        Continuation *_parent;
        Environment  *_environment;

    public:
        Continuation(Continuation *parent_):
            _parent(parent_),
            _environment(parent_->environment()) {}

        Continuation(Continuation *parent_, Environment *env_):
            _parent(parent_),
            _environment(env_) {}

        Environment *environment() const
            { return _environment; }

        Continuation *parent() const
            { return _parent; }

        virtual Continuation *send(Ptr value) = 0;
        virtual Continuation *step() = 0;
    };

    class Root: public Continuation
    {
        Ptr _result;

    public:
        Root(Environment *environment_):
            Continuation(nullptr, environment),
            result(nullptr) {}

        Ptr result() const
            { return _result; }

        virtual Continuation *send(Ptr value)
            { _result = value; return this; }

        virtual Continuation *step()
            { throw Exception(ERROR_runtime,
                              "Reached the root continuation."); }
    };

    /*! Applies a function */
    class Apply: public Continuation
    {
        Combiner *f;
        Ptr      args;

    public:
        Apply(Continuation *parent_, Combiner *f_):
            Continuation(parent_),
            f(f_),
            args(nullptr) {}

        virtual Continuation *send(Ptr value)
            { args = value; return this; }

        virtual Continuation *step()
            { return f->apply(parent(), args); }
    };

    /*! runs a program in a given environment.
     *
     * Creates a Root continuation, and one Apply continuation
     * evaluating the program. When the evaluation cascades back
     * to the root, return the result.
     */
    Ptr run(Ptr program, Environment *env)
    {
        auto root = new Root(env),
             cont = new Apply(root, &c_eval);

        cont->send(program);

        while (cont != root)
            cont = cont->step();

        return root->result();
    }
}
