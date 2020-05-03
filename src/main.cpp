#include <iostream>

#include "main.h"
#include "lexer/lexer.h"

int main(){
    Lexer lex;
    std::string s = "f";
    lex.tokenize(s);
    std::cout << lexemeSymbols[KEYWORD] << std::endl;
    return 0;
}