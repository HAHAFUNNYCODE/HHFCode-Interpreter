#ifndef LEXEMES_H
#define LEXEMES_H

#include <string>
#include <unordered_map>

namespace Lexer{

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
    static std::unordered_map<Token, std::string, TokenHash> lexemeNames;

    public:
        class InvalidTokenException{
            std::string message;

            public:
            InvalidTokenException(): message("A token was parsed that is unknown by the language"){};
            InvalidTokenException(std::string msg): message(msg){};
            InvalidTokenException(const InvalidTokenException& e): message(e.what()){}

            const char* what () const throw() {
                return message.c_str();
            }
        };

    public:
    inline Lexeme(): //Defaults to unknown
        type(UNKNOWN), value(""), column(-1), line(-1){}

    inline Lexeme(Token type): //Can be made with just a type, good for INVALID Lexemes
        type(type), value(""), column(-1), line(-1){}

    inline Lexeme(Token type, std::string value, int line, int column): //Provides all data
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

} //Namespace Lexer end


#endif //LEXEMES_H