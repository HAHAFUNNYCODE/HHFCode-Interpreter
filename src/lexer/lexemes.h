#ifndef LEXEMES_H
#define LEXEMES_H

#include <unordered_map>
#include <string>
#include <vector>

#ifndef TOKEN_E
#define TOKEN_E
enum Token{
    UNKNOWN,
    INVALID,
    FILEEND,

    IDENTIFIER, //Variables
    LITERAL,
    DECLARE,

    OPERATOR,
    SEPARATOR,
    KEYWORD,

    COMMENT
};

#endif //TOKEN_E


class TokenHash {
public: 
    inline size_t operator()(const Token& t) const
    {
        std::hash<int> inthash;
        return inthash(static_cast<int>(t));
    } 
};

class Lexeme{
    private:
    Token type;
    std::string value;
    int column, line;

    public:

    inline Lexeme():
        type(INVALID), value(""), column(-1), line(-1){}

    inline Lexeme(Token type, std::string value, int column, int line):
        type(type), value(value), column(column), line(line){}
    
    inline Token       getType()       {return type;}
    inline std::string getValue()      {return value;}
    inline int         getColumn()     {return column;}
    inline int         getLineNumber() {return line;}

    std::string getString();

    bool equals(const Lexeme& other);
    bool operator==(const Lexeme& other);
    bool operator!=(const Lexeme& other);

    inline operator int() const {
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

void initializeLexemes();

#endif //LEXEMES_H