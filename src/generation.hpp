#ifndef GENERATION_HPP_INCLUDED
#define GENERATION_HPP_INCLUDED
#include "parse.hpp"
#include<sstream>
#include<unordered_map>
class Generator
{
    public:
    inline explicit Generator(ProgNode prog)
    :m_prog(std::move(prog))
    {

    }
    inline void gen_expr(ExprNode &expr)
    {
        struct ExprVisitor
        {
            Generator *gen;
            void operator()(IntLitExprNode &int_lit_expr)
            {
                gen->m_output<<"mov rax,"<<int_lit_expr.int_lit.value.value()<<"\n";
                gen->push("rax");
            }
            void operator()(IdentExprNode &ident_expr)
            {
                if(!gen->m_varInfo.contains(ident_expr.ident.value.value()))
                {
                    std::cerr<<"Undeclared Identifier '"<<ident_expr.ident.value.value()<<"'"<<std::endl;
                    exit(EXIT_FAILURE);
                }
                std::stringstream offset;
                auto &varInfo=gen->m_varInfo.at(ident_expr.ident.value.value());
                offset<<"QWORD [rsp + "<<(gen->m_stackSize-1-varInfo.stack_loc)*8<<"]\n";
                gen->push(offset.str());
            }
        };
        ExprVisitor visitor {.gen=this};
        std::visit(visitor,expr.var);
    }
    inline void gen_stmt(StmtNode &stmt)
    {
        struct StmtVisitor
        {
            Generator *gen;
            void operator()(ExitStmtNode &exit_stmt)
            {
                gen->gen_expr(exit_stmt.expr);
                gen->m_output<<"mov rax,60\n";
                gen->pop("rdi");
                gen->m_output<<"syscall\n";
                
            }
            void operator()(LetStmtNode &let_stmt)
            {
                if(gen->m_varInfo.contains(let_stmt.ident.value.value()))
                {
                    std::cerr<<"Identifier already declared '"<<let_stmt.ident.value.value()<<"'"<<std::endl;
                    exit(EXIT_FAILURE);
                }
                gen->m_varInfo.insert({let_stmt.ident.value.value(),VarInfo {.stack_loc=gen->m_stackSize}});
                gen->gen_expr(let_stmt.expr);
            }
        };
        StmtVisitor visitor {.gen=this};
        std::visit(visitor,stmt.var);
    }
    inline std::string gen_prog()
    {
        this->m_output<<"global _start\n_start:\n";
        for(StmtNode stmt:this->m_prog.stmts)
        {
            this->gen_stmt(stmt);
        }
        this->m_output<<"mov rax, 60\n";
        this->m_output<<"mov rdi, 0\n";
        this->m_output<<"syscall\n";
        return this->m_output.str(); 
    }

    private:
    void push(const std::string& reg)
    {
        m_output<<"push "<<reg<<"\n";
        m_stackSize++;
    }
    void pop(const std::string &reg)
    {
        m_output<<"pop "<<reg<<"\n";
        m_stackSize--;
    }
    struct VarInfo
    {
        size_t stack_loc;
    };
    const ProgNode m_prog;
    std::stringstream m_output;
    size_t m_stackSize=0;
    std::unordered_map<std::string,VarInfo> m_varInfo;
};

#endif