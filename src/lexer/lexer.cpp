#include <regex>
#include <iostream>

#include "lexer.h"
// #include <iostream>

void Lexer::initialize(){
    initializeLexemes();
    loadKeywords();
    loadPatterns();
    initialized = true;
}

LexemeStream Lexer::tokenize(std::string &input){
    LexemeStream lexstream;
    std::smatch match;

    while (input.size()){
        switch (input[0])
        {
        case ' ':
        case '\n':
            input = input.substr(1,input.size()-1);
            break;
        default:
            // std::cout << "CUR: " << input << std::endl;
            for(LexerPattern p : patterns){
                bool found = false;
                // std::cout << "RUNNING: " << lexemeNames[p.type] << std::endl;
                if(std::regex_search(input, match, p.pattern)){
                    for(int i = 1; i <= p.groups; i++){
                        if(match.str(i) != ""){
                            lexstream.pushLexeme(
                                Lexeme(p.type, match.str(i), -1, -1)
                            );
                            found = true;
                            input = match.suffix().str();
                        }
                    }
                    if(found) break;
                }
            }
        }
    }

    lexstream.finish();
    return lexstream;
}

void Lexer::addPattern(Lexer::LexerPattern pat){
    patterns.push_back(pat);
}

void Lexer::addPattern(Token t, std::string p, int g){
    Lexer::LexerPattern pat(t,p,g);
    addPattern(pat);
}

void Lexer::loadPatterns(){
    // std::cout << "Adding: " << lexemeNames[COMMENT] << std::endl;
    addPattern(COMMENT,    lexemeSymbols[COMMENT],    2);
    // std::cout << "Adding: " << lexemeNames[KEYWORD] << std::endl;
    addPattern(KEYWORD,    lexemeSymbols[KEYWORD],    1);
    // std::cout << "Adding: " << lexemeNames[SEPARATOR] << std::endl;
    addPattern(SEPARATOR,  lexemeSymbols[SEPARATOR],  1);
    // std::cout << "Adding: " << lexemeNames[OPERATOR] << std::endl;
    addPattern(OPERATOR,   lexemeSymbols[OPERATOR],   1);
    // std::cout << "Adding: " << lexemeNames[LITERAL] << std::endl;
    addPattern(LITERAL,    lexemeSymbols[LITERAL],    1);
    // std::cout << "Adding: " << lexemeNames[DECLARE] << std::endl;
    addPattern(DECLARE,    lexemeSymbols[DECLARE],    1);
    // std::cout << "Adding: " << lexemeNames[IDENTIFIER] << std::endl;
    addPattern(IDENTIFIER, lexemeSymbols[IDENTIFIER], 1);
}