#ifndef TOKENIZATION_HPP_INCLUDED
#define TOKENIZATION_HPP_INCLUDED
#include<iostream>
#include<string>
#include<optional>
#include<vector>
enum class TokenType { exit, int_lit, semi ,open_paren ,close_paren , let, ident, eq, plus, star ,sub ,div};
struct Token
{
    TokenType type;
    std::optional<std::string> value;
};
inline std::optional<int> bin_prec(TokenType type)
{
    if(type==TokenType::plus)return 0;
    if(type==TokenType::sub)return 0;
    if(type==TokenType::star)return 1;
    if(type==TokenType::div)return 1;
    return {};
}
class Tokenizer
{
    public:
    inline explicit Tokenizer(std::string src)
    : m_src(std::move(src))
    {

    }
    inline std::vector<Token> tokenize()
    {
        std::string buf;
        std::vector<Token> tokens;
        while(peek().has_value())
        {
            if(std::isalpha(peek().value()))
            {
                buf.push_back(consume());
                while(peek().has_value() && std::isalnum(peek().value()))
                {
                    buf.push_back(consume());
                }
                if(buf=="exit")
                {
                    tokens.push_back({.type=TokenType::exit});
                    buf.clear();
                }
                else if(buf=="let")
                {
                    tokens.push_back({.type=TokenType::let});
                    buf.clear();
                }
                else
                {
                    tokens.push_back({.type=TokenType::ident ,.value=buf});
                    buf.clear();
                }
            }
            else if(std::isdigit(peek().value()))
            {
                buf.push_back(consume());
                while(peek().has_value() && std::isdigit(peek().value( )))
                {
                    buf.push_back(consume());
                }
                tokens.push_back({.type=TokenType::int_lit ,.value=buf});
                buf.clear();
            }
            else if(peek().value()=='(')
            {
                tokens.push_back({.type=TokenType::open_paren});
                consume();
            }
            else if(peek().value()==')')
            {
                tokens.push_back({.type=TokenType::close_paren});
                consume();
            }
            else if(peek().value()=='=')
            {
                tokens.push_back({.type=TokenType::eq});
                consume();
            }
            else if(peek().value()==';')
            {
                tokens.push_back({.type=TokenType::semi});
                consume();
            }
            else if(peek().value()=='+')
            {
                tokens.push_back({.type=TokenType::plus});
                consume();
            }
            else if(peek().value()=='*')
            {
                tokens.push_back({.type=TokenType::star});
                consume();
            }
            else if(peek().value()=='-')
            {
                tokens.push_back({.type=TokenType::sub});
                consume();
            }
            else if(peek().value()=='/')
            {
                tokens.push_back({.type=TokenType::div});
                consume();
            }
            else if(std::isspace(peek().value()))
            {
                consume();
            }
            else
            {
                std::cout<<"You have messed up"<<std::endl;
                exit(EXIT_FAILURE);
            }

        }
        m_index=0;
        return tokens;
    }

    

    private:
        [[nodiscard]] inline std::optional<char> peek(int offset=0) const
        {
            if(m_index+offset >= m_src.length())
            {
                return{};
            }
            else
            {
                return m_src.at(m_index+offset);
            }
        }

        inline char consume()
        {
            return m_src.at(m_index++);
        }
        const std::string m_src;
        size_t m_index=0;
        
};

#endif