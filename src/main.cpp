#include<fstream>
#include<iostream>
#include<optional>
#include<sstream>
#include<vector>
#include "tokenization.hpp"
#include "parse.hpp"
#include "generation.hpp"

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
    std::vector<Token> tokens=_tokenizer.tokenize();
    Parser _parser(tokens);
    std::optional<ProgNode> prog=_parser.parse_prog();
    Generator _generator(prog.value());
    std::string data=_generator.gen_prog();
    {
        std::fstream file("out.asm",std::ios::out);
        file<<data;
    }
    system("nasm -felf64 out.asm");
    system("ld out.o -o out");    
 
    return EXIT_SUCCESS; 
}