#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_set>
#include <unordered_map>

class LexemeStream;

class Lexer{
    //Member Variables
    private:
        bool initialized;

    //Member functions
    public:
        void initialize();
        LexemeStream tokenize(std::string& input);

        void loadPatterns();

};

class Trie{
    private:
    char value;
    std::unordered_set<char> charset;
    std::unordered_map<char, std::shared_ptr<Trie> > refs;
    // std::weak_ptr<Trie> backRef;
    bool isEnd;
    
    public:
    Trie(){}
    
    void addPattern(std::string);
    bool patternExists(std::string);
    std::shared_ptr<Trie> getRef(char c);
    inline bool isEnding() {return isEnd;}
    
    void print();

    private:
    void addPattern(std::string, int);
    void print(std::string);
};

#ifndef TOKEN_E
#define TOKEN_E
enum Token{
    UNKNOWN,
    INVALID,
    FILEEND,

    IDENTIFIER, //Variables
    LITERAL,
    DECLARE,

    OPERATOR,
    SEPARATOR,
    KEYWORD,

    COMMENT
};

#endif //TOKEN_E

class TokenHash;

//Externals
extern std::unordered_map<Token, std::string, TokenHash> lexemeNames;

const int NUMKW = 5;
extern std::array<std::string, NUMKW> keywords;
extern std::unordered_map<Token, std::vector<std::string>, TokenHash> knownSymbols;

#endif //LEXER_H