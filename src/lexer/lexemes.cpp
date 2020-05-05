#include <unordered_map>
#include <unordered_set>

#include "lexemes.h"

std::unordered_map<Token, std::string, TokenHash> lexemeNames;
std::unordered_map<Token, std::vector<std::string>, TokenHash> knownSymbols;
std::unordered_map<Token, std::unordered_set<char>, TokenHash> startChars;
std::unordered_set<char> whitespace, identifierSet;
char delimiter;
std::string lineCommentStart, blockCommentStart, blockCommentEnd;

std::string Lexeme::getString(){
    return lexemeNames[type] + '(' + value + ",ln" + std::to_string(line) + ')';
}

bool Lexeme::equals(const Lexeme& other){
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

void initializeLexemes(){

    lexemeNames = std::unordered_map<Token, std::string, TokenHash>({
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

    knownSymbols = std::unordered_map<Token, std::vector<std::string>, TokenHash> ({
        {OPERATOR,		{"+","-","*","/","%","=","==","<",">","<=",">=","!=", "++", "--", "+=", "-=", "*=", "/=", "%="}},
        {SEPARATOR,		{";",":","(",")","[","]","{","}"}},
        {KEYWORD,		{"if","else","elseif","for","while","var"}},
        {LITERAL,		{"true","false"}},
        {COMMENT,		{"//", "/*"}},
		{COMMENTEND,	{"*/"}}
    });

	startChars = std::unordered_map<Token, std::unordered_set<char>, TokenHash>({
        {OPERATOR,		{'+','-','*','/','%','=','<','>','!'}},
        {SEPARATOR,		{';',':','(',')','[',']','{','}', ','}},
        {KEYWORD,		{'i','e','f','w', 'v'}},
        {LITERAL,		{'t','f', '0','1','2','3','4','5','6','7','8','9','.','"'}},
		{COMMENT,		{'/'}},
		{COMMENTEND,	{'*'}}
	});

	whitespace = std::unordered_set<char>(
		{' ', '\t', '\n'}
	);

	identifierSet = std::unordered_set<char>(
		{
			'a','b','c','d','e','f','g','h','i','j','k','l','m',
			'n','o','p','q','r','s','t','u','v','w','x','y','z',
			'A','B','C','D','E','F','G','H','I','J','K','L','M',
			'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
			'0','1','2','3','4','5','6','7','8','9','_'
		}
	);

    delimiter = '\\';
	lineCommentStart = "//";
	blockCommentStart = "/*";
	blockCommentEnd = "*/";
}