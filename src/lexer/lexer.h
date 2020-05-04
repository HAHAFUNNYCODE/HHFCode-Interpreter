#ifndef LEXER_H
#define LEXER_H

#include <string>
#include "lexemestream.h"

class Lexer{
    //Member Variables
    private:;
    public:;

    //Member functions
    private:;
    public:
        LexemeStream tokenize(std::string& input);
        
};

//Externals
extern std::unordered_map<Token, std::string, TokenHash> lexemeNames;
extern std::unordered_map<Token, std::string, TokenHash> lexemeSymbols;

const int NUMKW = 5;
extern std::array<std::string, NUMKW> keywords;

#endif //LEXER_H