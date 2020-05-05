#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_set>
#include <unordered_map>

//Forward declarations
class LexemeStream;
class Trie;
class TokenHash;
class LexerUninitializedException;

#ifndef TOKEN_E
#define TOKEN_E
enum Token{ //Define token if not defined in lexemes.h
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
        bool initialized; //Checks initialized
        std::unordered_map<Token, Trie, TokenHash> patterns; //For matching known symbols

    //Member functions
    public:
        void initialize();
        LexemeStream tokenize(std::string& input);
        void loadPatterns();

    private:
        //Lexeme getters for the Lexer to use in tokenize()
        Lexeme getFromPattern(std::string& input, size_t* index, Token pat, bool needSep=true);
        Lexeme getIdentifier(std::string& input, size_t* index);
        Lexeme getComment(std::string& input, size_t* index);

        Lexeme getLiteral(std::string& input, size_t* index);
        Lexeme getString(std::string& input, size_t* index);
        Lexeme getNumber(std::string& input, size_t* index);
};

class Trie{ //A node for a Trie data structure
    private:
    char value; //The value of the node
    std::unordered_set<char> charset; //The characters that a node connects to
    std::unordered_map<char, std::shared_ptr<Trie> > refs; //Links to other nodes
    bool isEnd; //Flag for if a pattern ends at this node
    
    public:
    Trie(){}
    
    void addPattern(std::string); //Adds a pattern
    bool patternExists(std::string); //Checks if pattern exists (currently not implemented because wasn't needed)
    std::shared_ptr<Trie> getRef(char c); //Gets the node from the link of the character or nullptr if no node
    inline bool isEnding() {return isEnd;}
    inline bool isBottom() {return charset.empty();}
    inline char getValue() {return value;}
    
    // void print();

    private:
    void addPattern(std::string, int); //Recursive add pattern
    void print(std::string);
};

//Externals see lexemes.h
extern std::unordered_map<Token, std::string, TokenHash> lexemeNames;
extern std::unordered_map<Token, std::vector<std::string>, TokenHash> knownSymbols;
extern std::unordered_map<Token, std::unordered_set<char>, TokenHash> startChars;
extern std::unordered_set<char> whitespace, identifierSet;
extern char escapeChar;
extern std::string lineCommentStart, blockCommentStart, blockCommentEnd;

class InvalidTokenException : std::exception{ //Exception for an invalid token found when parsing
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