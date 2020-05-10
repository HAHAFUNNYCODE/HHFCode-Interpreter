#include <unordered_map>
#include <unordered_set>

#include "lexeme.h"

#if  defined(__APPLE__) || defined(__linux__) || defined(__unix__)
#define KWEMPHASIS "\e[92m"
#define VALUECOLOR "\e[94m"
#define RESETFMT "\e[0m"
#else
#define KWEMPHASIS ""
#define VALUECOLOR ""
#define RESETFMT ""
#endif

namespace Lexer{

std::unordered_map<Token, std::string, TokenHash> Lexeme::lexemeNames = {
    {UNKNOWN,		"UNKNOWN"},
    {INVALID,		"INVALID"},
    {FILEEND,       "EOF"},

    {IDENTIFIER,	"IDENTIFIER"},
    {LITERAL,		"LITERAL"},

    {OPERATOR,		"OPERATOR"},
    {SEPARATOR,		"SEPARATOR"},
    {KEYWORD,		"KEYWORD"},

    {COMMENT,		"COMMENT"},
};


///Gets a string representation of a Lexeme
/** Returns a string formatted as "Type(Value, Line, Column)".
 * @returns The string representation. */
std::string Lexeme::getString(){ //Returns a string representation of a lexeme
    return KWEMPHASIS + lexemeNames[type] + RESETFMT + 
    '(' + VALUECOLOR + value + RESETFMT + 
    ", line: " + std::to_string(line) + 
    " column: " + std::to_string(column) + ')';
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

} //Namespace Lexer end