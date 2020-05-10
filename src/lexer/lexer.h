#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "trie.h"

namespace Lexer{

//Forward declarations
class LexemeStream;
class Trie;
struct IndexTracker;
class TokenHash;

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
        std::unordered_map<Token, Trie, TokenHash> patterns; //For matching known symbols

    protected:
        bool initialized;
        std::unordered_map<Token, std::vector<std::string>, TokenHash> knownSymbols;
        std::unordered_map<Token, std::unordered_set<char>, TokenHash> startChars;
        std::unordered_set<char> whitespace, identifierSet;
        char escapeChar;
        std::string lineCommentStart, blockCommentStart, blockCommentEnd;

    public:
        class LexerUninitializedException;

    //Member functions
    public:
        Lexer(){}
        virtual ~Lexer(){};

        virtual void initialize(){};
        virtual void loadPatterns();
        virtual LexemeStream tokenize(std::string& input);

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

} //Namespace Lexer end

#endif //LEXER_H