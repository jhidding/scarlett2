#pragma once
#include "context.hh"
#include "environment.hh"
#include <deque>

namespace Scarlett
{
    class Continuation;
    
    /* 
     used in wishfull programming upto now:
        c_exec: create new child and pass list of tasks to it.
            Tasks may be: 
                - an expression to evaluate
            c_eval: evaluate single expression and push on stack
                - a function to apply
            c_apply: apply the argument to the current stack and push
                result on parent stack

        c_eval_seq: evaluate list of expressions and push value of
                last expression on stack, imp: new context, eval
     */
     
    /*************************************************************************
      base classes, generic interface
     *************************************************************************/
    enum C_Task_ID {
        C_EVAL, C_APPLY, C_PASS
    };
    
    enum C_Command_ID {        
        C_PUSH, C_SET,
        
        C_PARENT, C_PARENT_FILTER, C_CHILD, C_SIBLING,
        
        C_DEFINE, C_THIS, C_ENVIRONMENT
    };
    
    class C_Task: public Object
    {
    public:
        virtual C_Task_ID id() const = 0;
        
        virtual Cmd cmd() const
            { throw Exception(ERROR, "internal type error."); }
        virtual Ptr expr() const
            { throw Exception(ERROR, "internal type error."); }
        virtual Cmd apply(Ptr args) const
            { throw Exception(ERROR, "internal type error."); }
    };
    
    class C_Cmd: public Object
    {
    public:
        using C_Base = Context<C_Task, C_Cmd>;

        virtual C_Command_ID id() const = 0;
 
        virtual Ptr value() const
            { throw Exception(ERROR, "internal type error."); }

        virtual ptr<C_Base> make_child(ptr<C_Base> parent)
            { throw Exception(ERROR, "internal type error."); }

        virtual ptr<Environment> env() const
            { throw Exception(ERROR, "internal type error."); }            
    };

    /*************************************************************************
      Specifications for C_Cmd derivatives
     *************************************************************************/
    class C_CmdChild: public C_Cmd
    {
        ptr<Environment> _env;
        Ptr              _Q;
         
    public:
        C_CmdChild(ptr<Environment> env_, Ptr Q_):
            _env(env_), _Q(Q_) {}
            
        virtual C_Command_ID id() const
            { return C_CHILD; }

        virtual void gc_visit(GC const &gc) const noexcept
            { gc(_Q); gc(_env); }

        virtual ptr<C_Base> make_child(ptr<C_Base> parent)
            { return make_ptr<Continuation>(parent, _env, _Q); }
    };

    class C_CmdSibling: public C_CmdChild
    {
    public:
        using C_CmdChild::C_CmdChild;

        virtual C_Command_ID id() const
            { return C_SIBLING; }
    };
   
    template <typename ...Args>
    inline ptr<Cmd> c_exec(ptr<Environment> env, Args &&...rest)
    {
        Ptr Q = list(std::forward<Args>(rest)...);
        return make_ptr<C_CmdChild>(env, Q);
    }

    inline ptr<Cmd> c_sibling_seq(ptr<Environment> env, Ptr body)
    {
        Ptr Q = list(make_ptr<C_TaskApplyPrimitive>(car))
    }

    class C_CmdParent: public C_Cmd
    {
        Ptr     _value;

    public:
        C_CmdParent(Ptr value_):
            _value(value_) {}

        virtual C_Command_ID id() const
            { return C_PARENT; }

        virtual void gc_visit(GC const &gc) const noexcept
            { gc(_value); }

        virtual Ptr value() const
            { return _value; }
    };

    inline ptr<Cmd> c_parent(Ptr value)
    {
        return make_ptr<C_CmdParent>(value);
    }
    /*************************************************************************
      Specifications for C_Task derivatives
     *************************************************************************/ 
    class C_TaskEval: public C_Task
    {
        Ptr _expr;
        
    public:
        virtual void gc_visit(GC const &gc) const noexcept
            { gc(_expr); }
            
        C_TaskEval(Ptr expr_): _expr(expr_) {}
            
        virtual C_Task_ID id() const { return C_EVAL; }
        
        Ptr expr() const { return _expr; }
    };
    
    class C_TaskPass: public C_Task
    {
        Cmd _cmd;
        
    public:
        virtual void gc_visit(GC const &gc) const noexcept
            { gc(_cmd); }
            
        C_TaskPass(Cmd cmd_): _cmd(cmd_) {}
        
        virtual C_Task_ID id() const { return C_PASS; }
        
        Cmd cmd() const { return _cmd; }
    };
    
    class C_TaskApply: public C_Task
    {
    public:
        virtual C_Task_ID id() const { return C_APPLY; }
    };
    
    class C_TaskApplyPrimitive: public C_TaskApply
    {
        std::function<Ptr (Ptr)> f;
        
    public:
        template <typename T>
        C_TaskApplyPrimitive(T const &f_): f(f_) {}
        
        virtual Cmd operator()(Ptr args) const
            { return c_value(f(args)); }
    };
    
    class C_TaskApplyApplicative: public C_TaskApply
    {
        ptr<Applicative> f;
        
    public:
        C_TaskApplyApplicative(ptr<Applicative> f_):
            f(f_) {}
            
        virtual void gc_visit(GC const &gc) const noexcept
            { gc(f); }
            
        virtual Cmd operator()(Ptr args) const
            { return f->apply(args); }
    };
    
    class C_TaskApplyOperative: public C_TaskApply
    {
        ptr<Environment>    env;
        ptr<Operative>      f;
        
    public:
        C_TaskApplyOperative(ptr<Environment> env_, ptr<Operative> f_):
            env(env_), f(f_) {}
            
        virtual void gc_visit(GC const &gc) const noexcept
            { gc(f); }
        
        virtual Cmd operator()(Ptr args) const
            { return f->apply(env, args); }
    };
        
    inline bool is_command(Ptr a)
        { return is_type<Cmd>(a); }
        
    /* push argument on local continuation stack 
     */
    inline Cmd c_value(Ptr a)   
        { return Cmd(C_PUSH, a); }
    
    /* return to parent continuation, using argument f
     * to process the stack, identity function is used
     * if f equals nil.
     */
    inline Cmd c_return()
        { return Cmd(C_PARENT, nil); }
        
    inline Cmd c_return(Ptr f) 
        { return Cmd(C_PARENT_FILTER, f); }
    
    /* create new continuation frame
     */
    inline Cmd c_eval(Ptr env, Ptr body) 
        { return Cmd(C_CHILD, list(env, body)); }
    
    /* replace continuation frame */
    inline Cmd c_tail_eval(Ptr env, Ptr body) 
        { return Cmd(C_SIBLING, list(env, body)); }
        
    inline Cmd c_define(Ptr sym , Ptr value)
        { return Cmd(C_DEFINE, list(sym, value)); }
        
    inline Cmd c_current_continuation()
        { return Cmd(C_THIS, nil); }
        
    inline Cmd c_current_environment()
        { return Cmd(C_ENVIRONMENT, nil); }
        
    class Continuation: public Context<C_Task, Cmd>
    {
        ptr<Environment>   environment;
        Ptr        Q;
        Ptr        S;
        
    public:
        using Base = Context<C_Task, C_Cmd>;
        using Base::parent;
        
        Continuation(ptr<Base> parent_, ptr<Environment> env_, Ptr Q_):
            Base(parent_), environment(env_), Q(Q_)
        {}

        virtual void gc_visit(GC const &gc) const noexcept
        {
            Context::gc_visit(gc);
            gc(environment);
            gc(S);
            gc(Q);            
//            for (Ptr a : Q)
//                gc(a);
        }
            
        virtual bool empty() const
            { return is_nil(Q); }
            
        virtual T const &top() const
            { return cast<T>(car(Q)); }
            
        virtual void pop()
            { Q = cdr(Q); }
        
        virtual Cmd eval(Ptr const &expr) const;
        
        virtual ptr<Context> handle(Cmd const &return_value);
        
        virtual Ptr result() const
            { return S; }
    };
}

