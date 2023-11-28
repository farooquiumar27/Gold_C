#include<fstream>
#include<iostream>
#include<optional>
#include<sstream>
#include<vector>
#include "tokenization.hpp"
#include "parse.hpp"
#include "generation.hpp"
#include "arena.hpp"
int main(int argc,char *argv[])
{
    if(argc!=2)
    {
        std::cout<<"Incorrect usage .Correct usage is ..."<<std::endl;;
        std::cout<<"hydro <input.hy>"<<std::endl;
        return EXIT_FAILURE;
    }
    std::string contents;
    {
        std::stringstream content_stream;    
        std::fstream input(argv[1],std::ios::in);
        content_stream << input.rdbuf();
        contents=content_stream.str( );
    }
    Tokenizer _tokenizer(contents);
    //std::cout<<"Tokenization started"<<std::endl;
    std::vector<Token> tokens=_tokenizer.tokenize();
    //std::cout<<"Tokenization completed"<<std::endl;
    Parser _parser(tokens);
    //std::cout<<"Parsing started"<<std::endl;
    std::optional<ProgNode> prog=_parser.parse_prog();
    //std::cout<<"Parsing completed"<<std::endl;
    Generator _generator(prog.value());
    //std::cout<<"Generation started"<<std::endl;
    std::string data=_generator.gen_prog();
    //std::cout<<"Generation completed"<<std::endl;
    {
        std::fstream file("out.asm",std::ios::out);
        file<<data;
    }
    system("nasm -felf64 out.asm");
    system("ld out.o -o out");    
 
    return EXIT_SUCCESS; 
}