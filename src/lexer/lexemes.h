#ifndef LEXEMES_H
#define LEXEMES_H

#include <unordered_map>
#include <string>

enum Token;

struct Lexeme{
    private:
    Token type;
    std::string value;
    int column, line;

    public:
    Lexeme():
        type(INVALID), value(""), column(-1), line(-1){}

    Lexeme(Token type, std::string value, int column, int line):
        type(type), value(value), column(column), line(line){}

    Token       getType()       {return type;}
    std::string getValue()      {return value;}
    int         getColumn()     {return column;}
    int         getLineNumber() {return line;}

    bool EQ(const Lexeme& other){
        bool sameType = false; //Same type is assumed false
        bool sameVal = true; //Only care to check the value for certain tokens like Identifiers
        if (other.type == type) sameType == true;
        if (sameType && type==IDENTIFIER)
            if (other.value != value) sameVal = false;
        
        return sameType && sameVal;
    }

    bool operator==(const Lexeme& other){
        return EQ(other);
    }

    bool operator!=(const Lexeme& other){
        return !EQ(other);
    }

    operator int() const {
        switch(type){
            case UNKNOWN:
            case INVALID:
            case FILEEND:
                return 0;
            
            default:
                return 1;
        }
    }
};

enum Token{
    UNKNOWN,
    INVALID,
    FILEEND,

    IDENTIFIER, //Variables
    LITERAL,

    DECLARE,
    ASSIGN,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    PERC,

    LPAREN,
    RPAREN,
    LSQUARE,
	RSQUARE,
    LCURLY,
    RCURLY,

    IF,
    ELSE,
    ELSEIF,

    EQ,
    GT,
    LT,
    GTEQ,
    LTEQ,
    NEQ,

    AND,
    OR,
    NOT,
    LOOP,
    FCALL,
    FDECLARE
};

std::unordered_map<Token, std::string > lexemeNames ({
    {UNKNOWN,		"UNKNOWN"},
    {INVALID,		"INVALID"},
    {IDENTIFIER,	"IDENTIFIER"},
    {DECLARE,		"DECLARE"},
    {ASSIGN,		"ASSIGN"},
    {PLUS,			"PLUS"},
    {MINUS,			"MINUS"},
    {STAR,		    "STAR"},
    {SLASH,		    "SLASH"},
    {PERC,		    "PERC"},
    {LPAREN,		"LPAREN"},
    {RPAREN,		"RPAREN"},
    {EQ,		    "EQ"},
    {GT,		    "GT"},
    {LT,		    "LT"},
    {GTEQ,		    "GTEQ"},
    {LTEQ,		    "LTEQ"},
    {NEQ,		    "NEQ"},
    {AND,			"AND"},
    {OR,			"OR"},
    {NOT,			"NOT"},
    {LOOP,			"LOOP"},
    {FCALL,			"FCALL"},
    {FDECLARE,		"FDECLARE"},
    {LSQUARE,		"LSQUARE"},
	{RSQUARE,		"RSQUARE"},
    {LCURLY,		"LCURLY"},
	{RCURLY,		"RCURLY"},
    {IF,		    "IF"},
    {ELSE,		    "ELSE"},
    {ELSEIF,		"ELSEIF"},
    {LITERAL,		"LITERAL"},
    {FILEEND,       "EOF"}
});

std::unordered_map<Token, std::string> lexemeSymbols({
	{UNKNOWN,		""},
    {INVALID,		""},
    {IDENTIFIER,	"[\\w]"},
    {DECLARE,		"var"},
    {ASSIGN,		"="},
    {PLUS,			"+"},
    {MINUS,			"-"},
    {STAR,		    "*"},
    {SLASH,		    "/"},
    {PERC,		    "%"},
    {LPAREN,		"("},
    {RPAREN,		")"},
    {EQ,		    "=="},
    {GT,		    ">"},
    {LT,		    "<"},
    {GTEQ,		    ">="},
    {LTEQ,		    "<="},
    {NEQ,		    "!="},
    {AND,			"&&"},
    {OR,			"||"},
    {NOT,			"!"},
    {LOOP,			"for"},
    {FCALL,			"call"},
    {FDECLARE,		"function"},
    {LSQUARE,		"["},
	{RSQUARE,		"]"},
    {IF,		    "if"},
    {ELSE,		    "else"},
    {ELSEIF,		"elseif"},
    {LITERAL,		""},
    {FILEEND,       ""}
});

#endif //LEXEMES_H