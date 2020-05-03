#include "lexer.h"
#include "lexemes.h"
// #include <iostream>

LexemeStream Lexer::tokenize(std::string &input){
    LexemeStream lexstream;
    initializeLexemes();
    loadKeywords();

    return lexstream;
}