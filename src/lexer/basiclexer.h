#include "lexeme.h"
#include "lexer.h"

namespace Lexer{
    
class BasicLexer : public Lexer{
    public:
    BasicLexer(){
        initializeLexemes();
        loadPatterns();
        initialized = true;
    }

    private:
    void initializeLexemes(){ //Sets values to maps and sets for tokens
        knownSymbols = std::unordered_map<Token, std::vector<std::string>, TokenHash> ({ //Patterns to search for by token type
            {OPERATOR,		{"+","-","*","/","%","=","==","<",">","<=",">=","!=", "++", "--", "+=", "-=", "*=", "/=", "%="}},
            {SEPARATOR,		{";",":","(",")","[","]","{","}"}},
            {KEYWORD,		{"if","else","elseif","for","while","var"}},
            {LITERAL,		{"true","false"}},
            {COMMENT,		{"//", "/*"}},
            {COMMENTEND,	{"*/"}}
        });

        startChars = std::unordered_map<Token, std::unordered_set<char>, TokenHash>({ //The starting characters of the patterns
            {OPERATOR,		{'+','-','*','/','%','=','<','>','!'}},
            {SEPARATOR,		{';',':','(',')','[',']','{','}', ','}},
            {KEYWORD,		{'i','e','f','w', 'v'}},
            {LITERAL,		{'t','f', '0','1','2','3','4','5','6','7','8','9','.','"'}},
            {COMMENT,		{'/'}},
            {COMMENTEND,	{'*'}}
        });

        whitespace = std::unordered_set<char>( //Characters that are considered whitespace
            {' ', '\t', '\n'}
        );

        identifierSet = std::unordered_set<char>( //Characters that are valid for an identifier
            {
                'a','b','c','d','e','f','g','h','i','j','k','l','m',
                'n','o','p','q','r','s','t','u','v','w','x','y','z',
                'A','B','C','D','E','F','G','H','I','J','K','L','M',
                'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
                '0','1','2','3','4','5','6','7','8','9','_'
            }
        );

        escapeChar = '\\';
        lineCommentStart = "//";
        blockCommentStart = "/*";
        blockCommentEnd = "*/";
    }

};

} //Namespace Lexer end