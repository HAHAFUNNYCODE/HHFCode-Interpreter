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
    }
    auto start = std::chrono::high_resolution_clock::now();

    LexemeStream stream = lex.tokenize(inputStr);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration <double, std::milli> time = end - start;
    while (!stream.isDone())
    {
        std::cout << stream.next().getString() << std::endl;
    }

    std::cout << time.count() << "ms" << std::endl;
    
    return 0;
}