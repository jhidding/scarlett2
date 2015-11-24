#include "object.hh"

using namespace Scarlett;

Ptr const Scarlett::nil = nullptr;

Type Object_t("object");

Type const *Scarlett::Object::type() const
{
    return &Object_t;
}
