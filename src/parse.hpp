#ifndef PARSE_HPP_INCLUDED
#define PARSE_HPP_INCLUDED
#include "tokenization.hpp"
#include<variant>
struct IntLitExprNode
{
    Token int_lit;
};
struct IdentExprNode
{
    Token ident;
};
struct ExprNode
{
    std::variant<IntLitExprNode,IdentExprNode> var;
};
struct ExitStmtNode
{
    ExprNode expr;
};
struct LetStmtNode
{
    Token ident;
    ExprNode expr;
};
struct StmtNode
{
    std::variant<ExitStmtNode,LetStmtNode> var;
};
struct ProgNode
{
    std::vector<StmtNode> stmts;
};

struct ExitNode
{
    ExprNode expr;
};

class Parser
{
    public:
    inline explicit Parser(std::vector<Token> tokens)
    : m_tokens(std::move(tokens))
    {

    }
    inline std::optional<ExprNode> parse_expr()
    {
        if(peek().has_value() && peek().value().type==TokenType::int_lit)
        {
            IntLitExprNode int_lit_expr_node;
            int_lit_expr_node.int_lit=consume();
           return ExprNode {.var=int_lit_expr_node};
        }
        else if(peek().has_value() && peek().value().type==TokenType::ident)
        {
            IdentExprNode ident_expr_node;
            ident_expr_node.ident=consume();
            return ExprNode {.var=ident_expr_node};
        }
        else
        {
            return {};
        }
    }

    inline std::optional<StmtNode> parse_stmt()
    {
        if(peek().value().type==TokenType::exit && peek(1).has_value() && peek(1).value().type==TokenType::open_paren)
        {
            consume();
            consume();
            ExitStmtNode exit_stmt_node;
            if(auto expr_node=parse_expr())
            {
                exit_stmt_node.expr=expr_node.value();
            }
            else
            {
                std::cerr<<"Invalid Expression"<<std::endl;
                exit(EXIT_FAILURE);
            }
            if(peek().has_value() && peek().value().type==TokenType::close_paren)
            {
                consume();
            }
            else
            {
                std::cerr<<"Expected ')' at the end of expression"<<std::endl;
                exit(EXIT_FAILURE);
            }
            if(peek().has_value() && peek().value().type==TokenType::semi)
            {
                consume();
            }
            else
            {
                std::cerr<<"Missing ';' at the end of the statment"<<std::endl;
                exit(EXIT_FAILURE);
            }
            return StmtNode {.var=exit_stmt_node};
        }
        else if(peek().value().type==TokenType::let && peek(1).has_value() && peek(1).value().type==TokenType::ident && peek(2).has_value() && peek(2).value().type==TokenType::eq)
        {
            consume();
            LetStmtNode let_stmt_node;
            let_stmt_node.ident=consume();
            consume();
            if(auto expr_node = parse_expr())
            {
                let_stmt_node.expr=expr_node.value();
            }
            else
            {
                std::cerr<<"Invalid expression"<<std::endl;
            }
            if(peek().has_value() && peek().value().type==TokenType::semi)
            {
                consume();
            }
            else
            {
                std::cerr<<"Missing ';' at the end of the statment"<<std::endl;
                exit(EXIT_FAILURE);    
            }
            return StmtNode {.var=let_stmt_node};
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
};

#endif