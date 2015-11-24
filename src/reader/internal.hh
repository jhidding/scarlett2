#pragma once
#include "reader.hh"
#include "../system/context.hh"

#include <vector>
#include <functional>
#include <memory>

namespace Scarlett
{
    namespace Reader
    {
        using std::shared_ptr;

        struct Command;

        /** STRUCTURE DEFINITIONS *****************************************************/
        struct Rule
        {
            std::function<bool (char)> pred;
            std::vector<shared_ptr<Command const>> cmds;
        };

        using VRR = std::vector<Rule> const;

        enum OpCode {
            SUB_CONTEXT, PARENT_CONTEXT, SWITCH_CONTEXT,
            PUSH_CHAR, PRODUCE, PUT_BACK,
            IGNORE, PARSE_ERROR, DELEGATE };

        class Command
        {
        public:
            OpCode op;

            Command(OpCode op_): op(op_) {}
            virtual ~Command() noexcept {}

            /*! \brief Retrieves object send */
            virtual Ptr  get_obj() const { throw Exception(ERROR, "Internal syntax parser."); }

            /*! \brief Retrieves character to be pushed */
            virtual char get_chr() const { throw Exception(ERROR, "Internal syntax parser."); }

            /*! \brief Get the rules for a new context */
            virtual VRR &get_rls() const { throw Exception(ERROR, "Internal syntax parser."); }

            /*! \brief Create a new object from a string (this is usually a number parser) */
            virtual Ptr  mkf(std::string const &) const { throw Exception(ERROR, "Internal syntax parser."); }

            /*! \brief Apply some function to an object before returning it */
            virtual Ptr  cvf(Ptr)  const { throw Exception(ERROR, "Internal syntax parser."); }
        };

        typedef shared_ptr<std::vector<shared_ptr<Command const>>> CLstPtr;

        class Context: public Scarlett::Context<char, CLstPtr>
        {
            std::string const &source;
            size_t source_pos;

            VRR &rules;
            char current;

            std::string buffer;
            Ptr  context_result;

        public:
            using Base = Scarlett::Context<char, CLstPtr>;
            using Base::parent;

            Context(Base *parent_, std::string const &source_, size_t pos, VRR &rules_);

            bool empty() const;
            char const &top() const;
            void pop();
            void push_result(Ptr obj);
            void set_result(Ptr obj);
            void push_buffer(char ch);
            void put_back();
            Ptr result() const;

            CLstPtr eval(char const &ch) const;
            Base *handle(CLstPtr &return_value);
        };

        extern Ptr string_to_number(std::string const &s);

        extern VRR common_rules;

        extern VRR comment_rules;
        extern VRR comment_block_rules;
        extern VRR hash_in_comment_rules;
        extern VRR comment_block_bar_rules;

        extern VRR hash_rules;
        extern VRR vector_rules;
        extern VRR char_literal_rules;
        extern VRR special_rules;

        extern VRR number_rules;
        extern VRR symbol_rules;
        extern VRR string_rules;
        extern VRR string_char_rules;

        extern VRR list_rules;
        extern VRR alt_list_rules;

        extern VRR dot_rules;
        extern VRR alt_dot_rules;
        extern VRR ellipsis_rules;

        extern VRR improper_list_rules;
        extern VRR alt_improper_list_rules;
    }
}
