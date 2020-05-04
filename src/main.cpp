#include <iostream>
#include <fstream>

#include "main.h"
#include "lexer/lexer.h"

int main(int argc, char* argv[]){
    Lexer lex;
    lex.initialize();
    std::string inputStr;
    if (argc == 2){
        std::string fname = std::string(argv[1]);
        std::ifstream inputFile(fname);
        inputFile.seekg(0,std::ios::end);
        inputStr.resize(inputFile.tellg());
        inputFile.seekg(0,std::ios::beg);
        inputFile.read(&inputStr[0], inputStr.size());
        inputFile.close();
        std::cout << "HAVE FILE!" << std::endl;
        std::cout << inputStr << std::endl;
    }
    LexemeStream stream = lex.tokenize(inputStr);
    while (!stream.isDone())
    {
        std::cout << stream.next().getString() << std::endl;
    }
    
    return 0;
}