#include "numeric.hh"

using namespace Scarlett;

NumericType Infinity_t("inifinty", true);
NumericType Int_t("int", true);
NumericType Double_t("double", false);
NumericType Rational_t("rational", true);

Type const *Scarlett::Infinity::type() const
    { return &Infinity_t; }

Type const *Scarlett::Atom<int>::type() const
    { return &Int_t; }

Type const *Scarlett::Atom<double>::type() const
    { return &Double_t; }

Type const *Scarlett::Atom<rational>::type() const
    { return &Rational_t; }
