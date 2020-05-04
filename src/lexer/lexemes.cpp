#include "lexemes.h"

std::unordered_map<Token, std::string, TokenHash> lexemeNames;
std::unordered_map<Token, std::vector<std::string>, TokenHash> knownSymbols;

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
		{DECLARE,		{"var"}},
        {OPERATOR,		{"+","-","*","/","%","=","==","<",">","<=",">=","!="}},
        {SEPARATOR,		{";",":","(",")","[","]","{","}"}},
        {KEYWORD,		{"if","else","elseif","for","while"}},
    });
}