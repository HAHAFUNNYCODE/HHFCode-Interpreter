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


/**
 * @brief A functional class for lexer operations.
 * @details This class is used for doing lexical analysis on a string. It is a base class
 *  with default implementaions for how it does the analysis in the tokenize function.
 *  It is meant to be inherited with a constructor setting up the member data and calling
 *  loadPatterns to initialize the lexer.
 * */
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
        /**
         * @brief A constructor. 
         * @details The default constructor is blank, and starts unitialized.
        */
        Lexer():initialized(false){}
        virtual ~Lexer(){};

        //Optional overloads
        virtual void initialize(){};
        virtual void loadPatterns();
        virtual LexemeStream tokenize(std::string& input);

    private:
        //Lexeme getters for the Lexer to use in tokenize(), not inherited
        Lexeme getFromPattern(std::string& input, IndexTracker* index, Token pat, bool needSep=true);
        Lexeme getIdentifier(std::string& input, IndexTracker* index);
        Lexeme getComment(std::string& input, IndexTracker* index);

        Lexeme getLiteral(std::string& input, IndexTracker* index);
        Lexeme getString(std::string& input, IndexTracker* index);
        Lexeme getNumber(std::string& input, IndexTracker* index);

        std::string prettifyComment(std::string);
};


///A position tracker for the file.
struct IndexTracker{
    size_t index, line, col;

    ///Default constructor
    IndexTracker():index(0),line(1),col(1){}

    ///Value Constructor
    IndexTracker(size_t index, size_t line, size_t col): 
        index(index), line(line), col(col){}

    ///Copy Constructor
    IndexTracker(const IndexTracker& other):
        index(other.index), line(other.line), col(other.col){}

    ///Goes to next line
    void addLine(size_t n){
        line += n;
        col = 1;
    }

    ///Goes to next column
    void addCol(size_t n){
        col += n;
    }

    ///Increases index by 1
    size_t operator++(int n){
        index+=1;
        return index-1;
    }

    ///Decreases index by 1
    size_t operator--(int n){
        index-=1;
        return index+1;
    }

    ///Conversion to size_t
    inline operator size_t(){
        return index;
    }
};

} //Namespace Lexer end

#endif //LEXER_H