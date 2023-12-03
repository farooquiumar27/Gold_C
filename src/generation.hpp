#ifndef GENERATION_HPP_INCLUDED
#define GENERATION_HPP_INCLUDED
#include "parse.hpp"
#include<sstream>
#include<iostream>
class Generator
{
    public:
    inline explicit Generator(ProgNode prog)
    :m_prog(std::move(prog))
    {

    }
    inline void gen_term(TermNode *term)
    {
        struct TermVisitor
        {
            Generator *gen;
            void operator()(IntLitTermNode *int_lit_term)
            {
                gen->m_output<<"     mov rax, "<<int_lit_term->int_lit.value.value()<<"\n";
                gen->push("rax");
            }
            void operator()(IdentTermNode *ident_term)
            {
                if(!gen->containsVar(ident_term->ident.value.value()))
                {
                    std::cerr<<"Undeclared Identifier '"<<ident_term->ident.value.value()<<"'"<<std::endl;
                    exit(EXIT_FAILURE);
                }
                std::stringstream offset;
                auto var=gen->getVar(ident_term->ident.value.value());
                offset<<"QWORD [rsp + "<<(gen->m_stackSize-1-var.stack_loc)*8<<"]\n";
                gen->push(offset.str());
            }
            void operator()(ParenTermNode *paren_term)
            {
                gen->gen_expr(paren_term->expr);
            }
        };
        TermVisitor visitor {.gen=this};
        std::visit(visitor,term->var);
    }
    inline void gen_bin_expr(BinaryExprNode *binary_expr)
    {
        struct BinaryExprVisitor
        {
            Generator *gen;
            void operator()(AddBinaryExprNode *add)
            {
                gen->gen_expr(add->lhs);
                gen->gen_expr(add->rhs);
                gen->pop("rbx");
                gen->pop("rax");
                gen->m_output<<"     add rax, rbx\n";
                gen->push("rax");
            }
            void operator()(SubBinaryExprNode *sub)
            {
                gen->gen_expr(sub->lhs);
                gen->gen_expr(sub->rhs);
                gen->pop("rbx");
                gen->pop("rax");
                gen->m_output<<"     sub rax, rbx\n";
                gen->push("rax");
            }
            void operator()(MultiBinaryExprNode *multi)
            {
                gen->gen_expr(multi->lhs);
                gen->gen_expr(multi->rhs);
                gen->pop("rbx");
                gen->pop("rax");
                gen->m_output<<"     mul rbx\n";
                gen->push("rax");
            }
            void operator()(DivBinaryExprNode *div)
            {
                gen->gen_expr(div->lhs);
                gen->gen_expr(div->rhs);
                gen->pop("rbx");
                gen->pop("rax");
                gen->m_output<<"     div rbx\n";
                gen->push("rax");
            }
        };
        BinaryExprVisitor visitor {.gen=this};
        std::visit(visitor,binary_expr->var);
    }
    inline void gen_expr(ExprNode *expr)
    {
        struct ExprVisitor
        {
            Generator *gen;
            void operator()(TermNode *term_expr)
            {
                gen->gen_term(term_expr);
            }
            void operator()(BinaryExprNode *binary_expr)
            {
                gen->gen_bin_expr(binary_expr);
            }
        };
        ExprVisitor visitor {.gen=this};
        std::visit(visitor,expr->var);
    }
    inline void gen_scope(ScopeNode *scope)
    {
        begin_scope();
        for(StmtNode* stmt:scope->stmts)
        {
            gen_stmt(stmt);
        }
        end_scope();
    }
    inline void gen_stmt(StmtNode *stmt)
    {
        struct StmtVisitor
        {
            Generator *gen;
            void operator()(ExitStmtNode *exit_stmt)
            {
                gen->gen_expr(exit_stmt->expr);
                gen->m_output<<"     mov rax, 60\n";
                gen->pop("rdi");
                gen->m_output<<"     syscall\n";
                
            }
            void operator()(LetStmtNode *let_stmt)
            {
                if(gen->containsVar(let_stmt->ident.value.value()))
                {
                    std::cerr<<"Identifier already declared '"<<let_stmt->ident.value.value()<<"'"<<std::endl;
                    exit(EXIT_FAILURE);
                }
                gen->m_var.push_back(Var {.name=let_stmt->ident.value.value(),.stack_loc=gen->m_stackSize});
                gen->gen_expr(let_stmt->expr);
            }
            void operator()(ScopeNode *scope)
            {
                gen->gen_scope(scope);
            }
            void operator()(IfStmtNode *if_stmt)
            {
                gen->gen_expr(if_stmt->expr);
                gen->pop("rax");
                std::string label=gen->create_label();
                gen->m_output<<"     test rax, rax\n";
                gen->m_output<<"     jz "<<label<<"\n";
                gen->gen_scope(if_stmt->scope);
                gen->m_output<<" "<<label<<":\n";
            }
        };
        StmtVisitor visitor {.gen=this};
        std::visit(visitor,stmt->var);
    }
    inline std::string gen_prog()
    {
        this->m_output<<"global _start\n _start:\n";
        for(StmtNode* stmt:this->m_prog.stmts)
        {
            this->gen_stmt(stmt);
        }
        this->m_output<<"     mov rax, 60\n";
        this->m_output<<"     mov rdi, 0\n";
        this->m_output<<"     syscall\n";
        return this->m_output.str(); 
    }

    private:
    void push(const std::string& reg)
    {
        m_output<<"     push "<<reg<<"\n";
        m_stackSize++;
    }
    void pop(const std::string &reg)
    {
        m_output<<"     pop "<<reg<<"\n";
        m_stackSize--;
    }
    void begin_scope()
    {
        m_scopes_lb.push_back(m_var.size());
    }
    void end_scope()
    {
        size_t popCount=m_var.size()-m_scopes_lb.back();
        m_output<<"     add rsp, "<<popCount*8<<"\n";
        m_stackSize=m_stackSize-popCount;
        for(int i=0;i<popCount;i++)
        {
            m_var.pop_back();
        }
        m_scopes_lb.pop_back();
    }
    struct Var
    {
        std::string name;
        size_t stack_loc;
    };
    bool containsVar(std::string name)
    {
        for(Var var:m_var)
        {
            if(var.name==name)return true;
        }
        return false;
    }
    Var getVar(std::string name)
    {
        Var _var;
        for(Var var:m_var)
        {
            if(var.name==name)
            {
                _var.name=var.name;
                _var.stack_loc=var.stack_loc;
                break;
            }
        }
        return _var;
    }
    std::string create_label()
    {
        return std::string("label"+std::to_string(m_labelCount++));
    }
    const ProgNode m_prog;
    std::stringstream m_output;
    size_t m_stackSize=0;
    std::vector<Var> m_var;
    std::vector<size_t> m_scopes_lb;
    int m_labelCount=0;
};

#endif