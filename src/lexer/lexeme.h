#ifndef LEXEMES_H
#define LEXEMES_H

#include <string>
#include <unordered_map>

namespace Lexer{

///An enum type.
/**Represents the types of Tokens that the Lexer can find in a file*/
enum Token{
    UNKNOWN, /**< The token is currently unknown, but may still be valid*/
    INVALID, /**< The token is not supported*/
    FILEEND, /**< A token representing EOF*/

    IDENTIFIER, /**< Used to identify variables and functions in the language*/
    LITERAL, /**< Represent raw data literals*/

    OPERATOR, /**< Represents a language operator*/
    SEPARATOR, /**< Represents a language separator*/
    KEYWORD, /**< A keyword that is known by and reserved for the language*/

    COMMENT, /**< A line comment or comment blocks*/
    COMMENTEND /**< The end of a comment block*/
};

///A hash class.
/**Used for hashing a token into a hashmap.*/
class TokenHash {
public: 
    inline size_t operator()(const Token& t) const
    {
        std::hash<int> inthash;
        return inthash(static_cast<int>(t));
    } 
};

///A class for storing a lexeme.
/** Represents a lexeme that can be parsed by the lexer.*/
class Lexeme{
    private:
    Token type;
    std::string value;
    int column, line;
    static std::unordered_map<Token, std::string, TokenHash> lexemeNames;

    public:
        ///A Lexeme exception.
        /**An exception to be thrown when an Invalid Lexeme is found.*/
        class InvalidTokenException{
            std::string message;

            public:
            ///A constructor.
            /**Default constructor with message "A token was parsed that is unknown by the language."*/
            InvalidTokenException(): message("A token was parsed that is unknown by the language"){};

            InvalidTokenException(std::string msg): message(msg){};
            InvalidTokenException(const InvalidTokenException& e): message(e.what()){}

            ///Returns the message stored in the exception
            const char* what () const throw() {
                return message.c_str();
            }
        };

    public:
    inline Lexeme():
        type(UNKNOWN), value(""), column(-1), line(-1){}

    inline Lexeme(Token type):
        type(type), value(""), column(-1), line(-1){}

    inline Lexeme(Token type, std::string value, int line, int column): //Provides all data
        type(type), value(value), column(column), line(line){}

    ///Getter.
    /**Return the lexeme type. @returns The lexeme type.*/
    inline Token       getType()       {return type;}
    ///Getter.
    /**Return the lexeme value. @returns The lexeme value.*/
    inline std::string getValue()      {return value;}
    ///Getter.
    /**Return the lexeme column. @returns The lexeme column.*/
    inline int         getColumn()     {return column;}
    ///Getter.
    /**Return the lexeme line number. @returns The lexeme line number.*/
    inline int         getLineNumber() {return line;}

    std::string getString();

    bool equals(const Lexeme& other);
    bool operator==(const Lexeme& other);
    bool operator!=(const Lexeme& other);

    ///Convert Lexeme to int.
    /** A converter of the Lexeme based on the type.
     * The Lexeme evaluates to 0 if it is UNKNOWN, INVALID, or FILEEND, and 1 for everything else.
    */
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

} //Namespace Lexer end


#endif //LEXEMES_H