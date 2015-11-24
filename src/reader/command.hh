#pragma once
#include "internal.hh"

namespace Scarlett
{
    namespace Reader
    {
        inline shared_ptr<Command const> cmd_ignore()
            { return shared_ptr<Command const>(new Command(IGNORE)); }
        inline shared_ptr<Command const> cmd_parse_error()
            { return shared_ptr<Command const>(new Command(PARSE_ERROR)); }

        class Command_sub_context: public Command
        {
            VRR *rls;
            
        public:
            Command_sub_context(VRR *rls_):
                Command(SUB_CONTEXT), rls(rls_) {}
            VRR &get_rls() const { return *rls; }
        };

        inline shared_ptr<Command const> cmd_sub_context(VRR *rls)
            { return shared_ptr<Command const>(new Command_sub_context(rls)); }

        class Command_parent_context: public Command
        {
            Ptr (*_cvf)(Ptr);
        public:
            Command_parent_context(Ptr (*cvf_)(Ptr)):
                Command(PARENT_CONTEXT), _cvf(cvf_) {}

            Ptr cvf(Ptr a) const
                { return (*_cvf)(a); }
        };

        inline shared_ptr<Command const> cmd_parent_context(Ptr (*cvf)(Ptr))
            { return shared_ptr<Command const>(new Command_parent_context(cvf)); }

        class Command_switch_context: public Command
        {
            VRR *rls;

        public:
            Command_switch_context(VRR *rls_):
                Command(SWITCH_CONTEXT), rls(rls_) {}

            VRR &get_rls() const { return *rls; }
        };

        inline shared_ptr<Command const> cmd_switch_context(VRR *rls)
            { return shared_ptr<Command const>(new Command_switch_context(rls)); }

        class Command_push_char: public Command
        {
            char ch;

        public:
            Command_push_char(char ch_ = 0):
                Command(PUSH_CHAR), ch(ch_) {}

            char get_chr() const { return ch; }
        };

        inline shared_ptr<Command const> cmd_push_char(char ch = 0)
            { return shared_ptr<Command const>(new Command_push_char(ch)); }

        class Command_put_back: public Command
        {
            char ch;

        public:
            Command_put_back(char ch_ = 0):
                Command(PUT_BACK), ch(ch_) {}

            char get_chr() const { return ch; }
        };

        inline shared_ptr<Command const> cmd_put_back(char ch = 0)
            { return shared_ptr<Command const>(new Command_put_back(ch)); }

        class Command_produce: public Command
        {
            Ptr (*_mkf)(std::string const &);
        public:
            Command_produce(Ptr (*mkf_)(std::string const &)):
                Command(PRODUCE), _mkf(mkf_) {}

            Ptr mkf(std::string const &s) const
                { return (*_mkf)(s); }
        };

        inline shared_ptr<Command const> cmd_produce(Ptr (*mkf)(std::string const &))
            { return shared_ptr<Command const>(new Command_produce(mkf)); }

        class Command_delegate: public Command
        {
            VRR *rls;
        public:
            Command_delegate(VRR *rls_):
                Command(DELEGATE), rls(rls_) {}

            VRR &get_rls() const { return *rls; }
        };

        inline shared_ptr<Command const> cmd_delegate(VRR *rls)
            { return shared_ptr<Command const>(new Command_delegate(rls)); }
    }
}
