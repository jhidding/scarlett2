#include "internal.hh"
#include "command.hh"
#include "../system/symbol.hh"
#include "../system/tuple.hh"
#include "../system/vector.hh"
#include "../system/string.hh"
#include "../system/special.hh"

using namespace Scarlett;
using namespace Scarlett::Reader;
using Ctx = Scarlett::Reader::Context;

/** PREDICATES ****************************************************************/
#include <cctype>

std::function<bool (char)> one_of(std::string const &s)
{
    return [s] (char c) {
        for (char a : s)
            if (a == c) return true;
        return false;
    };
}

template <typename T>
std::function<bool (T)> equals(T v)
    { return [v] (T w) { return v == w; }; }

bool otherwise(char c) { return true; }

/** FILTERS *******************************************************************/
Ptr identity(Ptr p) { return p; }
Ptr nothing(Ptr p) { return nil; }

Ptr improper_list_reverse(Ptr p)
{
    Ptr s = car(p);
    p = cdr(p);

    while (p)
    {
        s = cons(car(p), s);
        p = cdr(p);
    }

    return s;
}

Ptr alt_improper_list_reverse(Ptr p)
{
    return cons("list"_s, improper_list_reverse(p));
}

Ptr alt_list_reverse(Ptr p)
{
    return cons("list"_s, list_reverse(p));
}

Ptr reverse_list_to_vector(Ptr p)
{
    auto s = new Vector();

    while (p)
    {
        s->push_back(car(p));
        p = cdr(p);
    }

    return s;
}

Ptr string_concatenate(Ptr p)
{
    std::string s = "";

    while (p)
    {
        s = cast<String>(car(p))->value + s;
        p = cdr(p);
    }

    return new String(s);
}

Ptr make_symbol(std::string const &s)
{
    //return make_ptr<Symbol const>(s);
    return new Symbol(s);
}

Ptr make_special(std::string const &s)
{
    //return make_ptr<Special const>(s);
    return new Special(s);
}

Ptr make_char_literal(std::string const &s)
{
    return new Atom<char>(s[0]);
}

Ptr make_string(std::string const &s)
{
    return new String(s);
}

Ptr make_ellipsis(std::string const &s)
{
    return new Special("ellipsis");
}

/** RULES *********************************************************************/
VRR Scarlett::Reader::common_rules = {
    {equals('('),  {cmd_sub_context(&list_rules)}},
    {equals('['),  {cmd_sub_context(&alt_list_rules)}},
    {equals('"'),  {cmd_sub_context(&string_rules)}},
    {equals(';'),  {cmd_sub_context(&comment_rules)}},
    {equals('#'),  {cmd_sub_context(&hash_rules)}},
    {isdigit,      {cmd_put_back(),
                    cmd_sub_context(&number_rules)}},
    {isspace,      {cmd_ignore()}},
    {otherwise,    {cmd_put_back(),
                    cmd_sub_context(&symbol_rules)}}
};

VRR Scarlett::Reader::list_rules = {
    {equals(']'),  {cmd_parse_error()}},
    {equals(')'),  {cmd_parent_context(&list_reverse)}},
    {equals('.'),  {cmd_sub_context(&dot_rules)}},
    {otherwise,    {cmd_delegate(&common_rules)}}
};

VRR Scarlett::Reader::dot_rules = {
    {equals('.'),  {cmd_switch_context(&ellipsis_rules)}},
    {isspace,      {cmd_switch_context(&improper_list_rules)}},
    {isdigit,      {cmd_switch_context(&number_rules),
                    cmd_push_char('.'),
                    cmd_push_char()}},
    {otherwise,    {cmd_parse_error()}}
};

VRR Scarlett::Reader::improper_list_rules = {
    {equals(']'),  {cmd_parse_error()}},
    {equals(')'),  {cmd_parent_context(&car),
                    cmd_parent_context(&improper_list_reverse)}},
    {otherwise,    {cmd_delegate(&common_rules)}}
};

VRR Scarlett::Reader::alt_dot_rules = {
    {equals('.'),  {cmd_switch_context(&ellipsis_rules)}},
    {isspace,      {cmd_switch_context(&alt_improper_list_rules)}},
    {isdigit,      {cmd_switch_context(&number_rules),
                    cmd_push_char('.'),
                    cmd_push_char()}},
    {otherwise,    {cmd_parse_error()}}
};

VRR Scarlett::Reader::ellipsis_rules = {
    {equals('.'),  {cmd_push_char()}},
    {otherwise,    {cmd_put_back(),
                    cmd_produce(&make_ellipsis),
                    cmd_parent_context(&car)}}
};

VRR Scarlett::Reader::alt_improper_list_rules = {
    {equals(')'),  {cmd_parse_error()}},
    {equals(']'),  {cmd_parent_context(&car),
                    cmd_parent_context(&alt_improper_list_reverse)}},
    {otherwise,    {cmd_delegate(&common_rules)}}
};

VRR Scarlett::Reader::alt_list_rules = {
    {equals(')'),  {cmd_parse_error()}},
    {equals(']'),  {cmd_parent_context(&alt_list_reverse)}},
    {equals('.'),  {cmd_sub_context(&alt_dot_rules)}},
    {otherwise,    {cmd_delegate(&common_rules)}}
};

VRR Scarlett::Reader::string_rules = {
    {equals('\\'), {cmd_produce(&make_string),
                    cmd_sub_context(&string_char_rules)}},
    {equals('"'),  {cmd_produce(&make_string),
                    cmd_parent_context(&string_concatenate)}},
    {otherwise,    {cmd_push_char()}}
};

VRR Scarlett::Reader::string_char_rules = {
    {otherwise,    {cmd_push_char(),
                    cmd_produce(&make_string),
                    cmd_parent_context(&car)}}
};

VRR Scarlett::Reader::comment_rules = {
    {equals('\n'), {cmd_parent_context(nothing)}},
    {otherwise,    {cmd_ignore()}}
};

VRR Scarlett::Reader::comment_block_rules = {
    {equals('|'),  {cmd_sub_context(&comment_block_bar_rules)}},
    {equals('#'),  {cmd_sub_context(&hash_in_comment_rules)}},
    {otherwise,    {cmd_ignore()}}
};

VRR Scarlett::Reader::hash_in_comment_rules = {
    {equals('|'),  {cmd_sub_context(&comment_block_rules)}},
    {otherwise,    {cmd_parent_context(&nothing)}}
};

VRR Scarlett::Reader::comment_block_bar_rules = {
    {equals('#'),  {cmd_parent_context(&nothing),
                    cmd_parent_context(&nothing)}},
    {otherwise,    {cmd_parent_context(&nothing)}}
};

VRR Scarlett::Reader::hash_rules = {
    {equals('|'),  {cmd_switch_context(&comment_block_rules)}},
    {equals('\\'), {cmd_switch_context(&char_literal_rules)}},
    {equals('('),  {cmd_switch_context(&vector_rules)}},
    {isspace,      {cmd_parse_error()}},
    {otherwise,    {cmd_switch_context(&special_rules)}}
};

VRR Scarlett::Reader::vector_rules = {
    {equals(')'),  {cmd_parent_context(&reverse_list_to_vector)}},
    {otherwise,    {cmd_delegate(&common_rules)}}
};

VRR Scarlett::Reader::char_literal_rules = {
    {isspace,      {cmd_produce(&make_char_literal),
                    cmd_parent_context(&car)}},
    {otherwise,    {cmd_push_char()}}
};

VRR Scarlett::Reader::number_rules = {
    {one_of("\t\n ()[]{}\""),
                   {cmd_put_back(),
                    cmd_produce(&string_to_number),
                    cmd_parent_context(&car)}},
    {otherwise,    {cmd_push_char()}}
};

VRR Scarlett::Reader::symbol_rules = {
    {one_of("\t\n ()[]{}\""),
                   {cmd_put_back(),
                    cmd_produce(&make_symbol),
                    cmd_parent_context(&car)}},
    {otherwise,    {cmd_push_char()}}
};

VRR Scarlett::Reader::special_rules = {
    {one_of("\t\n ()[]{}\""),
                   {cmd_produce(&make_special),
                    cmd_parent_context(&car),
                    cmd_put_back()}},
    {otherwise,    {cmd_push_char()}}
};
