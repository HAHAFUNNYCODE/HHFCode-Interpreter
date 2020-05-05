#include <string>
#include <iostream>

#include "lexemes.h"
#include "lexemestream.h"
#include "lexer.h"

// #include <iostream>

void Lexer::initialize(){
    initializeLexemes();
    loadPatterns();
    initialized = true;
}

LexemeStream Lexer::tokenize(std::string &input){
    LexemeStream lexstream;

    size_t index = 0;

    while(index < input.size()){
        char curC = input[index];
        //Check for Whitespace
        if(whitespace.find(curC) != whitespace.end()){
            index++;
            continue;
        }

        //Check for keywords
        if(startChars[KEYWORD].find(curC) != startChars[KEYWORD].end()){
            Lexeme lex = getFromPattern(input, &index, KEYWORD);
            Token type = lex.getType();
            if (type == INVALID)
                throw InvalidTokenException();
            if (type == KEYWORD){
                lexstream.pushLexeme(lex);
                continue;
            }
        }

        //Check for literals
        if(startChars[LITERAL].find(curC) != startChars[LITERAL].end()){
            Lexeme lex = getLiteral(input, &index);
            Token type = lex.getType();
            if (type == INVALID)
                throw InvalidTokenException();
            if (type == LITERAL){
                lexstream.pushLexeme(lex);
                continue;
            }
        }

        //Check for operators
        if(startChars[OPERATOR].find(curC) != startChars[OPERATOR].end()){
            Lexeme lex = getFromPattern(input, &index, OPERATOR, false);
            Token type = lex.getType();
            if (type == INVALID)
                throw InvalidTokenException();
            if (type == OPERATOR){
                lexstream.pushLexeme(lex);
                continue;
            }
        }

        //Check for separators
        if(startChars[SEPARATOR].find(curC) != startChars[SEPARATOR].end()){
            std::string charS(1,curC);
            Lexeme lex(SEPARATOR, charS, -1,-1);
            lexstream.pushLexeme(lex);
            index++;
            continue;
        }

        //Check for Identifiers
        if(identifierSet.find(curC) != identifierSet.end() && !('0' <= curC && curC <= '9')){
            Lexeme lex = getIdentifier(input, &index);
            Token type = lex.getType();
            if (type == INVALID)
                throw InvalidTokenException();
            if (type == IDENTIFIER){
                lexstream.pushLexeme(lex);
                continue;
            }
        }

        //If it gets here, either the interpreter is bad or the program is bad.
        throw InvalidTokenException("Nothing matched to language tokens.");

    } 


    lexstream.finish();
    return lexstream;
}


void Lexer::loadPatterns(){
    for(auto symbols : knownSymbols){
        Trie t;
        for(std::string symbol : symbols.second){
            t.addPattern(symbol);
        }
        patterns[symbols.first] = t;
    }
}

void Trie::addPattern(std::string p){
    addPattern(p,0);
}

void Trie::addPattern(std::string p, int index){
    if (index > 0){
        value = p[index-1];
    }
    if(index >= p.size()){
        isEnd = true;
        return;
    }
    char c = p[index];

    if(charset.find(c) == charset.end()){
        charset.insert(c);
        refs[c] = std::make_shared<Trie>();
    }
    refs[c]->addPattern(p, index+1);
}


//Do Later, not sure if needed
bool Trie::patternExists(std::string pat){
    return false;
}

std::shared_ptr<Trie> Trie::getRef(char c){
    if (charset.find(c) == charset.end())
        return std::shared_ptr<Trie>(nullptr);
    else
        return refs[c];
}

// void Trie::print(){
//     for(char c : charset){
//         std::string s(1, c);
//         refs[c]->print(s);
//     }
// }

// void Trie::print(std::string s){
//     if(charset.empty()){
//         std::cout << s << std::endl;
//         return;
//     }

//     if (isEnd)
//         std::cout << s << std::endl;

//     for(char c : charset){
//         refs[c]->print(s+c);
//     }
// }

Lexeme Lexer::getFromPattern(std::string& input, size_t* index, Token pat, bool needSep){
    size_t indexfallback = *index;
    std::shared_ptr<Trie> node = patterns[pat].getRef(input[(*index)++]), prev;
    std::string value = "";

    while (node && *index < input.size()){
        value += node->getValue();
        prev = node;
        node = node->getRef(input[(*index)++]);
    }

    if(*index < input.size()){
        (*index)--;
        if (prev && prev->isEnding()){
            if(!needSep){
                return Lexeme(pat, value, -1, -1);
            }

            if( (whitespace.find(input[*index]) != whitespace.end() ) || 
                startChars[SEPARATOR].find(input[*index]) != startChars[SEPARATOR].end()){
                    return Lexeme(pat, value, -1, -1);
            } 

        }
    }

    else{
        if(node && node->isEnding()){
            return Lexeme(pat, value + node->getValue(), -1, -1);
        }

        if(!node && prev->isEnding()){
            return Lexeme(pat, value, -1, -1);
        }
    }

    *index = indexfallback;
    return Lexeme();
}

Lexeme Lexer::getLiteral(std::string& input, size_t* index){
    char start = input[*index];
    if(start == '"'){
        return getString(input, index);
    } else if(start == '.' || ('0' <= start && start <= '9') || start == '-'){
        return getNumber(input, index);
    }

    return getFromPattern(input, index, LITERAL);
}

Lexeme Lexer::getString(std::string& input, size_t* index){
    size_t indexfallback = *index;
    std::string str(1,input[(*index)++]);
    bool foundEnd = false;
    while(!foundEnd && *index < input.size()){
        if(input[*index] == '"' && input[(*index) - 1] != delimiter){
            foundEnd = true;
        }
        str += input[(*index)++];
    }

    if(foundEnd)
        return Lexeme(LITERAL, str, -1, -1);

    if(*index >= input.size())
        *index = indexfallback;
        return Lexeme(INVALID);

    return Lexeme(INVALID);
}

Lexeme Lexer::getNumber(std::string& input, size_t* index){
    size_t indexfallback = *index;
    std::string number;
    bool passedPeriod = false;

    if(input[*index] == '-'){
        number += '-';
        (*index)++;
    }
    
    while(*index < input.size()){
        char cur = input[*index];
        if('0' <= cur && cur <= '9'){
            number += cur;
            (*index)++;
            continue;
        }

        if(cur == '.'){
            if(!passedPeriod){
                number += '.';
                (*index)++;
                passedPeriod = true;
                continue;
            } else{
                return Lexeme(INVALID);
            }
        }

        if(whitespace.find(cur) != whitespace.end() || startChars[SEPARATOR].find(cur) != startChars[SEPARATOR].end()){
            return Lexeme(LITERAL, number, -1, -1);
        }

        return Lexeme(INVALID);
    }
    
    if(*index == input.size())
        return Lexeme(LITERAL, number, -1, -1);

    *index = indexfallback;
    return Lexeme(INVALID);
}

Lexeme Lexer::getIdentifier(std::string& input, size_t* index){
    size_t indexfallback = *index;
    std::string value(1,input[(*index)++]);

    while(*index < input.size() && identifierSet.find(input[*index]) != identifierSet.end()){
        value += input[(*index)++];
    }

    if(*index >= input.size()){
        return Lexeme(IDENTIFIER, value, -1, -1);
    }

    if(whitespace.find(input[*index]) != whitespace.end() ||
       startChars[SEPARATOR].find(input[*index]) != startChars[SEPARATOR].end() ||
       startChars[OPERATOR].find(input[*index]) != startChars[OPERATOR].end() ){
           return Lexeme(IDENTIFIER, value, -1, -1);
    }

    *index = indexfallback;
    return Lexeme(UNKNOWN, "", -1, -1);
}