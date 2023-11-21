#include<iostream>
#include<fstream>
#include<sstream>
#include<optional>
#include<vector>
using namespace std;
enum class TokenType
{
    _return,
    int_lit,
    semi 
};
struct Token
{
    TokenType type;
    optional<string> value;
};
vector<Token> tokenize(string &str)
{
    vector<Token> tokens;
    string buf;
    for(int i=0;i<str.size();i++)
    {
        if(isalpha(str[i]))
        {
            buf.push_back(str[i]);
            i++;
            while(isalnum(str[i]))
            {
                buf.push_back(str[i]);
                i++;
            }
            i--;
            if(buf=="return")
            {
                tokens.push_back({.type=TokenType::_return});
                buf.clear( );
                continue;
            }
            else
            {
                cout<<"You have medded up"<<endl;
                cout<<buf<<endl;
                exit(EXIT_FAILURE);
            }
        }
        else if(isdigit(str[i]))
        {
            buf.push_back(str[i]);
            i++;
            while(isdigit(str[i]))
            {
                buf.push_back(str[i]);
                i++;
            }
            i--;
            tokens.push_back({.type=TokenType::int_lit , .value=buf});
            continue;
            buf.clear( );
        }
        else if(str[i]==';')
        {
            tokens.push_back({.type=TokenType::semi});
            continue;
        }
        else if(isspace(str[i]))
        {
            continue;
        }
        else
        {
            cout<<"You have messed up"<<endl;
            cout<<"_2"<<endl;
            exit(EXIT_FAILURE);
        }
    }
    return tokens;
}
string tokens_to_asm(vector<Token> tokens)
{
    stringstream output;
    output<<"global _start\n_start:\n";
    for(int i=0;i<tokens.size();i++)
    {
        if(tokens[i].type==TokenType::_return)
        {
            if(i+1<tokens.size() && tokens[i+1].type==TokenType::int_lit)
            {
                if(i+2<tokens.size() && tokens[i+2].type==TokenType::semi)
                {
                    output<<"mov rax ,60\n";
                    output<<"mov rdi ,"<<tokens[i+1].value.value()<<"\n";
                    output<<"syscall";
                }
            }
        }
    }
    return output.str();
}
int main(int argc,char *argv[])
{
    if(argc!=2)
    {
        cout<<"Incorrect usage .Correct usage is ..."<<endl;;
        cout<<"hydro <input.hy>"<<endl;
        return EXIT_FAILURE;
    }
    string contents;
    {
        stringstream content_stream;    
        fstream input(argv[1],ios::in);
        content_stream << input.rdbuf();
        contents=content_stream.str( );
    }
    vector<Token> tokens=tokenize(contents);
    {
        fstream file("out.asm",ios::out);
        file<<tokens_to_asm(tokens);
    }
    system("nasm -felf64 out.asm");
    system("ld out.o -o out");    
    return EXIT_SUCCESS; 
}