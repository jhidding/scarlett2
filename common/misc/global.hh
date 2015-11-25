#pragma once
#include <memory>
#include <string>
#include <map>

namespace Misc
{
    template <typename T>
    class Global: public T
    {
        typedef std::map<std::string, T *> tmap;
        static std::unique_ptr<tmap> _dir;

        public:
            static tmap &dir()
            {
                if (not _dir)
                    _dir = std::unique_ptr<tmap>(new tmap);

                return *_dir;
            }

            template <typename ...Args>
            Global(std::string const &name, Args &&...args):
                T(std::forward<Args>(args)...)
            {
                dir()[name] = this;
            }
    };

    template <typename T>
    std::unique_ptr<std::map<std::string, T *>> Misc::Global<T>::_dir;
}
