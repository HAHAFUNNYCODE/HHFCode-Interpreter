#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_set>
#include <unordered_map>

class LexemeStream;
class Trie;
class TokenHash;

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
    COMMENTEND
};

#endif //TOKEN_E


class Lexer{
    //Member Variables
    private:
        bool initialized;
        std::unordered_map<Token, Trie, TokenHash> patterns;

    //Member functions
    public:
        void initialize();
        LexemeStream tokenize(std::string& input);
        void loadPatterns();

    private:
        Lexeme getFromPattern(std::string& input, size_t* index, Token pat, bool needSep=true);
        Lexeme getIdentifier(std::string& input, size_t* index);
        Lexeme getComment(std::string& input, size_t* index);

        Lexeme getLiteral(std::string& input, size_t* index);
        Lexeme getString(std::string& input, size_t* index);
        Lexeme getNumber(std::string& input, size_t* index);
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
    inline bool isBottom() {return charset.empty();}
    inline char getValue() {return value;}
    
    // void print();

    private:
    void addPattern(std::string, int);
    void print(std::string);
};

//Externals
extern std::unordered_map<Token, std::string, TokenHash> lexemeNames;
extern std::unordered_map<Token, std::vector<std::string>, TokenHash> knownSymbols;
extern std::unordered_map<Token, std::unordered_set<char>, TokenHash> startChars;
extern std::unordered_set<char> whitespace, identifierSet;
extern char delimiter;
extern std::string lineCommentStart, blockCommentStart, blockCommentEnd;

class InvalidTokenException : std::exception{
    private:
    std::string message;
    public:
    InvalidTokenException(): message("A token was parsed that is unknown by the language"){};
    InvalidTokenException(std::string msg): message(msg){};

    const char* what () const throw() {
        return message.c_str();
    }
};

#endif //LEXER_H