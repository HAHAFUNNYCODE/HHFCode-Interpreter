#include <unordered_map>
#include <unordered_set>

#include "lexemes.h"

std::unordered_map<Token, std::string, TokenHash> lexemeNames; //Names for the types of tokens
std::unordered_map<Token, std::vector<std::string>, TokenHash> knownSymbols; //Patterns for each types of tokens
std::unordered_map<Token, std::unordered_set<char>, TokenHash> startChars; //First character for the symbols of the tokens
std::unordered_set<char> whitespace, identifierSet; //Useful sets for 
char escapeChar; //What will be used for escaped characters like \n
std::string lineCommentStart, blockCommentStart, blockCommentEnd;

std::string Lexeme::getString(){ //Returns a string representation of a lexeme
    return lexemeNames[type] + '(' + value + ",ln" + std::to_string(line) + ')';
}

bool Lexeme::equals(const Lexeme& other){ //Checks Lexeme equality
        bool sameType = false; //Same type is assumed false
        bool sameVal = true; //Only care to check the value for certain tokens like Identifiers
        sameType = (other.type == type);
        sameVal = (other.value == value);

        return sameType && sameVal;
    }

bool Lexeme::operator==(const Lexeme& other){
    return equals(other);
}

bool Lexeme::operator!=(const Lexeme& other){
    return !equals(other);
}

void initializeLexemes(){ //Sets values to maps and sets for tokens

    lexemeNames = std::unordered_map<Token, std::string, TokenHash>({ //Names for string representation
        {UNKNOWN,		"UNKNOWN"},
        {INVALID,		"INVALID"},
        {FILEEND,       "EOF"},

        {IDENTIFIER,	"IDENTIFIER"},
        {LITERAL,		"LITERAL"},
        {DECLARE,		"DECLARE"},

        {OPERATOR,		"OPERATOR"},
        {SEPARATOR,		"SEPARATOR"},
        {KEYWORD,		"KEYWORD"},

        {COMMENT,		"COMMENT"},
    });

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