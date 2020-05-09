#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "trie.h"

//Forward declarations
class LexemeStream;
class Trie;
struct IndexTracker;
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
        Lexeme getFromPattern(std::string& input, IndexTracker* index, Token pat, bool needSep=true);
        Lexeme getIdentifier(std::string& input, IndexTracker* index);
        Lexeme getComment(std::string& input, IndexTracker* index);

        Lexeme getLiteral(std::string& input, IndexTracker* index);
        Lexeme getString(std::string& input, IndexTracker* index);
        Lexeme getNumber(std::string& input, IndexTracker* index);
};


struct IndexTracker{
    size_t index, line, col;

    IndexTracker():index(0),line(1),col(1){}

    IndexTracker(size_t index, size_t line, size_t col): 
        index(index), line(line), col(col){}

    IndexTracker(const IndexTracker& other):
        index(other.index), line(other.line), col(other.col){}

    void addLine(size_t n){
        line += n;
        col = 1;
    }

    void addCol(size_t n){
        col += n;
    }

    size_t operator++(int n){
        index+=1;
        return index-1;
    }

    size_t operator--(int n){
        index-=1;
        return index+1;
    }

    inline operator size_t(){
        return index;
    }
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