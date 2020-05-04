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

    lexstream.finish();
    return lexstream;
}


void Lexer::loadPatterns(){
    for(auto symbols : knownSymbols){
        Trie t;
        for(std::string symbol : symbols.second){
            t.addPattern(symbol);
        }
        t.print();
        std::cout << "//==================//" << std::endl;
    }
}

void Trie::addPattern(std::string p){
    addPattern(p,0);
}

void Trie::addPattern(std::string p, int index){
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

bool Trie::patternExists(std::string pat){
    return false;
}

std::shared_ptr<Trie> Trie::getRef(char c){
    return std::shared_ptr<Trie>(nullptr);
}

void Trie::print(){
    for(char c : charset){
        std::string s(1, c);
        refs[c]->print(s);
    }
}

void Trie::print(std::string s){
    if(charset.empty()){
        std::cout << s << std::endl;
        return;
    }

    if (isEnd)
        std::cout << s << std::endl;

    for(char c : charset){
        refs[c]->print(s+c);
    }
}