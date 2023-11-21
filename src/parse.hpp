#ifndef PARSE_HPP_INCLUDED
#define PARSE_HPP_INCLUDED
#include "tokenization.hpp"
struct NodeExpr
{
    Token int_lit;
};
struct NodeExit
{
    NodeExpr expr;
};

class Parser
{
    public:
    inline explicit Parser(std::vector<Token> tokens)
    : m_tokens(std::move(tokens))
    {

    }
    inline std::optional<NodeExpr> parse_expr()
    {
        if(peak().has_value() && peak().value().type==TokenType::int_lit)
        {
            return NodeExpr {.int_lit=consume()};
        }
        else
        {
            return {};
        }
    }
    inline std::optional<NodeExit> parse()
    {
        NodeExit nodeExit;
        if(peak().has_value() && peak().value().type==TokenType::exit)
        {
            consume();
            if(auto nodeExpr=parse_expr())
            {
                nodeExit=NodeExit {.expr=nodeExpr.value()};
            }
            else
            {
            std::cerr<<"Invalid expression"<<std::endl;
            exit(EXIT_FAILURE);
            }
            if(peak().has_value() && peak().value().type==TokenType::semi)
            {
                consume();
            }
            else
            {
            std::cerr<<"Invalid expression"<<std::endl;
            exit(EXIT_FAILURE);
            }
                        
        }
        else
        {
            std::cerr<<"Invalid expression"<<std::endl;
            exit(EXIT_FAILURE);
        }
        m_index=0;
        return nodeExit;
    }

    
    private:
    [[nodiscard]] inline std::optional<Token> peak(int ahead=1)
    {
        if(m_index+ahead <= m_tokens.size())
        {
            return m_tokens.at(m_index);
        }
        else
        {
            return {};
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