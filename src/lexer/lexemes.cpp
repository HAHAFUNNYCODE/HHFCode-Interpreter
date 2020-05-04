#include "lexemes.h"

std::unordered_map<Token, std::string, TokenHash> lexemeNames;
std::unordered_map<Token, std::string, TokenHash> lexemeSymbols;

const int NUMKW = 5;
std::array<std::string, NUMKW> keywords;

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
    keywords = {
        "if",
        "elseif",
        "else",
        "for",
        "while"
    }; 

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

    lexemeSymbols = std::unordered_map<Token, std::string, TokenHash>({
        {IDENTIFIER,	"^(\\w+)"},
        {LITERAL,		"^((?:\\d*\\.\\d+|\\d+\\.?\\d*)|\".*?\")"},
		{DECLARE,		"^(var)\\b"},

        {OPERATOR,		"^(\\+|-|\\*|/|%|==|=|<|>|<=|>=|!=|\\|\\||&&)"},
        {SEPARATOR,		"^(\\(|\\)|\\[|\\]|\\{|\\}|;|:|,|\t)"},
        {KEYWORD,		""},

        {COMMENT,		"^(?:\\/\\/([^\n]*)|\\/\\*((?:.|\\s)*?)\\*\\/)"},
    });
}

void loadKeywords(){
    std::string regex = "^(";
    for(int i = 0; i < NUMKW; i++){
        regex += keywords[i];
        if (i < NUMKW-1) 
            regex += '|';
    }
    regex += ")\\b"; 
    lexemeSymbols[KEYWORD] = regex;
}