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

Ptr Scarlett::read(std::string const &s)
{
    return list_reverse(run_context<Ctx>(s, 0, list_rules));
}

/*! \namespace Reader
 *
 * Each time we interpret a character, we pass it through a list
 * of predicates. If there is a match, then the corresponding set
 * of Context commands are executed.
 *
 * ReaderContext:
 *   Rules   rules;
 *   Context parent;
 *   string  buffer;
 *   Ptr     objlst;
 *
 * - PARENT_CONTEXT, return to previous context, filtering the
 *     objlst through the argument function and push it to
 *     the parent objlst.
 * - SWITCH_CONTEXT, change context to a new one, but use parent
 *     of current context. Equivalent to tail-call in scheme.
 * - SUB_CONTEXT, make a new context frame and push it on the
 *     context stack.
 *
 * - PUSH_CHAR, add current character to buffer; if parameter is
 *     given, push that character in stead.
 * - PRODUCE, convert buffer to new Object and push to objlst.
 * - PUT_BACK, put character back on stream for reevaluation.
 *
 * - DELEGATE, special command used to pass character to other
 *     set of rules. This command is taken care of at evaluation time,
 *     all the others at context handling time.
 * - IGNORE, do nothing.
 * - ERROR, throw an exception.
 */

/** CONTEXT CONSTRUCTOR *******************************************************/
Ctx::Context(Context::Base *parent_, std::string const &source_, size_t pos, VRR &rules_):
    Base(parent_),
    source(source_),
    source_pos(pos),
    rules(rules_)
{}

/** CONTEXT ONLINERS **********************************************************/
bool Ctx::empty() const
    { return source_pos == source.size(); }

char const &Ctx::top() const
    {// std::cerr << ". " << source[source_pos] << ": ";
      return source[source_pos]; };

void Ctx::pop()
    { ++source_pos; };

void Ctx::push_result(Ptr obj)
    { if (obj) context_result = cons(obj, context_result); }

void Ctx::set_result(Ptr obj)
    { context_result = obj; }

void Ctx::push_buffer(char ch)
    { buffer += ch; }

void Ctx::put_back()
    { --source_pos; }

Ptr Ctx::result() const
    { return context_result; };

/** EVALUATOR *****************************************************************/

CLstPtr get_commands(VRR &rules, char ch)
{
    auto x = CLstPtr();

    for (auto const &r : rules)
    {
        if (r.pred(ch))
        {
            if (r.cmds[0]->op == DELEGATE)
                return get_commands(r.cmds[0]->get_rls(), ch);

            for (std::shared_ptr<Command const> c : r.cmds)
            {
                if (c->op == PUSH_CHAR and c->get_chr() == 0)
                {
                    x->push_back(cmd_push_char(ch));
                    continue;
                }

                x->push_back(c);
            }

            return x;
        }
    }

    throw Exception(ERROR_syntax, "None of the predicates match character: ", ch);
}

std::shared_ptr<std::vector<std::shared_ptr<Command const>>> Ctx::eval(char const &ch) const
{
    return get_commands(rules, ch);
}

/** BYTE-CODE HANDLER *********************************************************/

Ctx::Base *Ctx::handle(
        CLstPtr &return_value)
{
    Ctx *cc = this, *pc;

    for (std::shared_ptr<Command const> c : *return_value)
    {
        switch (c->op)
        {
            case SUB_CONTEXT:
//                std::cerr << "[sub_context]\n";
                cc = new Ctx(cc, source, source_pos, c->get_rls());
                break;

            case PARENT_CONTEXT:
                pc = dynamic_cast<Ctx *>(cc->parent());
                pc->push_result(c->cvf(cc->result()));
                pc->source_pos = source_pos;
//                std::cerr << "[parent_context " << ::repr(cc->result()) << " -> " << ::repr(pc->result()) << "]\n";
                cc = pc;
                break;

            case SWITCH_CONTEXT:
//                std::cerr << "[switch_context]\n";
                cc = new Ctx(parent(), source, source_pos, c->get_rls());
                break;

            case PUSH_CHAR:
                cc->push_buffer(c->get_chr());
//                std::cerr << "[push_char '" << c->get_chr() << "' -> " << cc->buffer << "]\n";
                break;

            case PRODUCE:
//                std::cerr << "[produce " << buffer << " -> " << ::repr(c->mkf(cc->buffer)) << "]\n";
                cc->context_result = cons(c->mkf(cc->buffer), cc->context_result);
                break;

            case PUT_BACK:
//                std::cerr << "[put_back]\n";
                cc->put_back();
                break;

            case IGNORE:
//                std::cerr << "[ignore]\n";
                break;

            case PARSE_ERROR:
                throw Exception(ERROR_syntax, "Syntax error.");

            case DELEGATE:
                throw Exception(ERROR, "Impossible codepoint.");
         }
    }

    return cc;
}
