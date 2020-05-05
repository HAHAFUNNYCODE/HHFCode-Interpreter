#ifndef LEXEMES_H
#define LEXEMES_H

#include <string>
#include <vector>

#ifndef TOKEN_E
#define TOKEN_E
enum Token{ //An enum for identifying tokens
    UNKNOWN,
    INVALID,
    FILEEND,

    IDENTIFIER, //Variables
    LITERAL,
    DECLARE,

    OPERATOR,
    SEPARATOR,
    KEYWORD,

    COMMENT,
    COMMENTEND
};

#endif //TOKEN_E


class TokenHash { //Hash function class for token (for maps)
public: 
    inline size_t operator()(const Token& t) const
    {
        std::hash<int> inthash;
        return inthash(static_cast<int>(t));
    } 
};

class Lexeme{ //Represents a lexeme that can be parsed by the lexer
    private:
    Token type;
    std::string value;
    int column, line;

    public:
    inline Lexeme(): //Defaults to unknown
        type(UNKNOWN), value(""), column(-1), line(-1){}

    inline Lexeme(Token type): //Can be made with just a type, good for INVALID Lexemes
        type(type), value(""), column(-1), line(-1){}

    inline Lexeme(Token type, std::string value, int column, int line): //Provides all data
        type(type), value(value), column(column), line(line){}
    
    //Getters
    inline Token       getType()       {return type;}
    inline std::string getValue()      {return value;}
    inline int         getColumn()     {return column;}
    inline int         getLineNumber() {return line;}

    std::string getString(); //returns string representation

    bool equals(const Lexeme& other);
    bool operator==(const Lexeme& other);
    bool operator!=(const Lexeme& other);

    inline operator int() const { //Convert to integer for checking boolean
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

void initializeLexemes(); //Sets values

#endif //LEXEMES_H