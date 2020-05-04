#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <regex>
#include "lexemestream.h"

class Lexer{
    //Member Variables
    public:
        struct LexerPattern{
            Token type;
            std::regex pattern;
            int groups;

            inline LexerPattern(LexerPattern const &copy): type(copy.type), pattern(copy.pattern), groups(copy.groups){}
            inline LexerPattern(Token t, std::string p, int g): type(t), pattern(std::regex(p)), groups(g){}
            inline LexerPattern(std::pair<Token, std::string> p): type(p.first), pattern(std::regex(p.second)), groups(1){}
        };
    private:
        bool initialized;
        std::vector<LexerPattern> patterns;

    //Member functions
    public:
        void initialize();
        LexemeStream tokenize(std::string& input);
  
        void addPattern(LexerPattern pat);
        void addPattern(Token t, std::string pat, int g);
        void loadPatterns();

};

//Externals
extern std::unordered_map<Token, std::string, TokenHash> lexemeNames;
extern std::unordered_map<Token, std::string, TokenHash> lexemeSymbols;

const int NUMKW = 5;
extern std::array<std::string, NUMKW> keywords;

#endif //LEXER_H