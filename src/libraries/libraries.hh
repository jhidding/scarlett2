#pragma once
#include "../../common/misc/global.hh"
#include "../system/environment.hh"

namespace Scarlett {
    using Misc::Global;

    class Library {
        Environment *env;
        Ptr depends;

    public:
        typedef std::function<void (std::string const &, Ptr)> Binder;

        template <typename F>
        Library(Ptr needs, F init_);

        Environment *load();

    private:
        std::function<void (Binder)> init;
        bool init_done;
    };

    inline Environment *load_library(std::string const &name) {
        return Global<Library>::dir()[name]->load();
    }

    template <typename F>
    Library::Library(Ptr depends_, F init_):
        depends(depends_),
        init(init_),
        init_done(false) {}

    inline Environment *Library::load() {
        if (!init_done) {
            env = new Environment(map(load_library, depends));
            init([this] (std::string const &name, Ptr value) {
                env->bind(name, value);});
            init_done = true;
        }

        return env;
    }
}
