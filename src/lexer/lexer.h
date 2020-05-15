#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <functional>

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
        typedef std::unordered_map<Token, std::function<Lexeme(std::string&, IndexTracker*)>, TokenHash> TokenFuncMap;
        
        std::unordered_map<Token, Trie, TokenHash> patterns; //For matching known symbols

    protected:
        bool initialized;
        std::unordered_map<Token, std::vector<std::string>, TokenHash> knownSymbols;
        std::unordered_map<Token, std::unordered_set<char>, TokenHash> startChars;
        std::unordered_set<char> whitespace, identifierSet;
        char escapeChar;
        std::string lineCommentStart, blockCommentStart, blockCommentEnd;

    public:
        class LexerUninitializedException; //See below

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
        virtual void tokenize(LexemeStream&, std::string&);

    protected:
        //Lexeme getters for the Lexer to use in tokenize(), not inherited
        bool validateLexeme(Token type, Lexeme& lex);

        Lexeme getIdentifier(std::string& input, IndexTracker* index);
        Lexeme getComment(std::string& input, IndexTracker* index);
        Lexeme getKeyword(std::string& input, IndexTracker* index);
        Lexeme getOperator(std::string& input, IndexTracker* index);
        Lexeme getSeparator(std::string& input, IndexTracker* index);

        Lexeme getLiteral(std::string& input, IndexTracker* index);
        Lexeme getString(std::string& input, IndexTracker* index);
        Lexeme getNumber(std::string& input, IndexTracker* index);

        Lexeme getFromPattern(std::string& input, IndexTracker* index, Token pat, bool needSep=true);

        std::string prettifyComment(std::string);
};

///A Lexer::Lexer Exception
class Lexer::LexerUninitializedException : std::exception{ //Thrown if the lexer is used without initialization
    const char* what () const throw() {
        return "The Lexer has not been initialized.";
    }
};

} //Namespace Lexer end

#endif //LEXER_H