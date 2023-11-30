#ifndef PARSE_HPP_INCLUDED
#define PARSE_HPP_INCLUDED
#include "tokenization.hpp"
#include<variant>
#include "arena.hpp"
struct BinaryExprNode;
struct ExprNode;
struct IntLitTermNode
{
    Token int_lit;
};
struct IdentTermNode
{
    Token ident;
};
struct ParenTermNode
{
    ExprNode *expr;
};
struct TermNode
{
    std::variant<IntLitTermNode*,IdentTermNode*,ParenTermNode*> var;
};
struct ExprNode
{
    std::variant<TermNode*,BinaryExprNode*> var;
};
struct AddBinaryExprNode
{
    ExprNode *lhs;
    ExprNode *rhs;
};
struct MultiBinaryExprNode
{
    ExprNode *lhs;
    ExprNode *rhs;
};
struct SubBinaryExprNode
{
    ExprNode *lhs;
    ExprNode *rhs;
};
struct DivBinaryExprNode
{
    ExprNode *lhs;
    ExprNode *rhs;
};
struct BinaryExprNode
{
    std::variant<AddBinaryExprNode*,MultiBinaryExprNode*,SubBinaryExprNode*,DivBinaryExprNode*> var;
};

struct ExitStmtNode
{
    ExprNode *expr;
};
struct LetStmtNode
{
    Token ident;
    ExprNode *expr;
};
struct StmtNode
{
    std::variant<ExitStmtNode*,LetStmtNode*> var;
};
struct ProgNode
{
    std::vector<StmtNode*> stmts;
};


class Parser
{
    public:
    inline explicit Parser(std::vector<Token> tokens)
    : m_tokens(std::move(tokens)) ,m_allocator(1024*1024*4)  //4mb
    {

    }
    inline std::optional<TermNode*> parse_term()
    {
        if(peek().has_value() && peek().value().type==TokenType::int_lit)
        {
            TermNode *term;
            term=m_allocator.alloc<TermNode>();            
            IntLitTermNode* int_lit_term;
            int_lit_term=m_allocator.alloc<IntLitTermNode>();
            int_lit_term->int_lit=consume();
            term->var=int_lit_term;
           return term;
        }
        else if(peek().has_value() && peek().value().type==TokenType::ident)
        {
            TermNode *term;
            term=m_allocator.alloc<TermNode>();            
            IdentTermNode *ident_term;
            ident_term=m_allocator.alloc<IdentTermNode>();
            ident_term->ident=consume();
            term->var=ident_term;
            return term;
        }
        else if(peek().has_value() && peek().value().type==TokenType::open_paren)
        {
            consume();
            TermNode *term;
            term=m_allocator.alloc<TermNode>();
            std::optional<ExprNode*> expr=parse_expr();
            if(!expr.has_value())
            {
                std::cerr<<"Invalid term Expression"<<std::endl;
                exit(EXIT_FAILURE);
            }
            try_comsume(TokenType::close_paren,"Expected '(' at the end of term expression");
            ParenTermNode *paren_term;
            paren_term=m_allocator.alloc<ParenTermNode>();
            paren_term->expr=expr.value();
            term->var=paren_term;
            return term;
        }
        else
        {
            return {};
        }
    }
    inline std::optional<ExprNode*> parse_expr(int min_prec=0)
    {
        std::optional<TermNode*> lhs_term=parse_term();
        if(!lhs_term.has_value())
        {
            return {};
        }
        auto lhs_expr=m_allocator.alloc<ExprNode>();
        lhs_expr->var=lhs_term.value();
        
        while(true)
        {
            std::optional<Token> cur_tok=peek();
            std::optional<int> prec;
            if(cur_tok.has_value())
            {
                prec=bin_prec(cur_tok->type);
                if(!prec.has_value() ||  prec.value()<min_prec)
                {
                    break;
                }
            }
            else
            {
                    break;
            }
            Token op=consume();
            int nxt_min_prec=prec.value()+1;
            auto rsh_expr=parse_expr(nxt_min_prec);
            if(!rsh_expr.has_value())
            {
                std::cerr<<"Unable to parse expression"<<std::endl;
                exit(EXIT_FAILURE);
            }
            ExprNode *expr;
            expr=m_allocator.alloc<ExprNode>();
            BinaryExprNode* binary_expr;
            binary_expr=m_allocator.alloc<BinaryExprNode>();
            if(op.type==TokenType::plus)
            {
                expr->var=lhs_expr->var;
                AddBinaryExprNode* add;
                add=m_allocator.alloc<AddBinaryExprNode>();
                add->lhs=expr;
                add->rhs=rsh_expr.value();
                binary_expr->var=add;
                lhs_expr->var=binary_expr;
            }
            else if(op.type==TokenType::star)
            {
                expr->var=lhs_expr->var;
                MultiBinaryExprNode* multi;
                multi=m_allocator.alloc<MultiBinaryExprNode>();
                multi->lhs=expr;
                multi->rhs=rsh_expr.value();
                binary_expr->var=multi;
                lhs_expr->var=binary_expr;
            }
            else if(op.type==TokenType::sub)
            {
                expr->var=lhs_expr->var;
                SubBinaryExprNode* sub;
                sub=m_allocator.alloc<SubBinaryExprNode>();
                sub->lhs=expr;
                sub->rhs=rsh_expr.value();
                binary_expr->var=sub;
                lhs_expr->var=binary_expr;
            }
            else if(op.type==TokenType::div)
            {
                expr->var=lhs_expr->var;
                DivBinaryExprNode* div;
                div=m_allocator.alloc<DivBinaryExprNode>();
                div->lhs=expr;
                div->rhs=rsh_expr.value();
                binary_expr->var=div;
                lhs_expr->var=binary_expr;
            }
        }

        return lhs_expr;
        
    }

    inline std::optional<StmtNode *> parse_stmt()
    {
        if(peek().value().type==TokenType::exit && peek(1).has_value() && peek(1).value().type==TokenType::open_paren)
        {
            consume();
            consume();
            ExitStmtNode *exit_stmt_node;
            exit_stmt_node=m_allocator.alloc<ExitStmtNode>();
            if(auto expr_node=parse_expr())
            {
                exit_stmt_node->expr=expr_node.value();
            }
            else
            {
                std::cerr<<"Invalid Expression"<<std::endl;
                exit(EXIT_FAILURE);
            }
            try_comsume(TokenType::close_paren,"Excepted ')' ");
            try_comsume(TokenType::semi,"Excepted ';' ");
            StmtNode *stmt_node;
            stmt_node=m_allocator.alloc<StmtNode>();
            stmt_node->var=exit_stmt_node;
            return stmt_node;
        }
        else if(peek().value().type==TokenType::let && peek(1).has_value() && peek(1).value().type==TokenType::ident && peek(2).has_value() && peek(2).value().type==TokenType::eq)
        {
            consume();
            LetStmtNode *let_stmt_node;
            let_stmt_node=m_allocator.alloc<LetStmtNode>();
            let_stmt_node->ident=consume();
            consume();
            if(auto expr_node = parse_expr())
            {
                let_stmt_node->expr=expr_node.value();
            }
            else
            {
                std::cerr<<"Invalid expression"<<std::endl;
            }
            try_comsume(TokenType::semi,"Excepted ';' ");
            StmtNode *stmt_node;
            stmt_node=m_allocator.alloc<StmtNode>();
            stmt_node->var=let_stmt_node;
            return stmt_node;
        }
        else
        {
            return {};
        }
    }
    
    inline std::optional<ProgNode> parse_prog()
    {
        ProgNode prog_node;
        while(peek().has_value())
        {
            if(auto stmt_node=parse_stmt())
            {
                prog_node.stmts.push_back(stmt_node.value());
            }
            else
            {
                std::cerr<<"Invalid statment"<<std::endl;
                exit(EXIT_FAILURE);
            }
        }
        return prog_node;
    }

    
    private:
    inline Token try_comsume(TokenType type,std::string errmsg)
    {
        if(peek().has_value() && peek().value().type==type)
        {
            return consume();
        }
        else
        {
            std::cerr<<errmsg<<std::endl;
            exit(EXIT_FAILURE);
        }  
    }
    
    [[nodiscard]] inline std::optional<Token> peek(int offset=0)
    {
        if(m_index+offset >= m_tokens.size())
            {
                return{};
            }
            else
            {
                return m_tokens.at(m_index+offset);
            }
    }
    inline Token consume()
    {
        return m_tokens.at(m_index++);
    }
    const std::vector<Token> m_tokens;
    std::size_t m_index=0;
    ArenaAllocator m_allocator;
};

#endif