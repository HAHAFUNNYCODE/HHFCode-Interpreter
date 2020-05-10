#ifndef LEXEMESTREAM_H
#define LEXEMESTREAM_H

#include <queue>
#include <exception>
#include <memory>

#include "lexeme.h"

namespace Lexer{

///A stream for receiving lexemes in order.
/** Allows for Lexeme objects to be inserted and retrieved in a FIFO order.*/
class LexemeStream { //Stream responsible for outputing lexemes.
    private:
    std::queue<Lexeme> stream;
    bool open, done;

    public:
    ///A LexerStream Exception
    class ClosedLexStreamException : std::exception { //When trying to push to closed stream
    const char* what () const throw() {
        return "Cannot update closed stream.";
        }
    };

    ///A LexerStream Exception
    class FinishedLexStreamException : std::exception { //When trying to access values from stream with no values
        const char* what () const throw() {
            return "Tried to access value from ended stream";
        }
    };

    public:
    ///A constructor
    /** The constructor where the stream starts open and not done.*/
    LexemeStream(): open(true), done(false){}

    ///Adds Lexeme to stream
    void pushLexeme(Lexeme lex){
        if (open) //If the stream is available to be pushed to, do so
            stream.push(lex);
        else //Otherwise throw excpetion
            throw ClosedLexStreamException();
    }

    ///Finishes and closes stream.
    void finish(){ 
        pushLexeme( Lexeme(FILEEND, "EOF", -1, -1) );
        open = false;
    }

    ///Gets the next Lexeme in the stream
    Lexeme next(){
        if (done || stream.empty()) //If finished then throws exception
            throw FinishedLexStreamException();

        Lexeme nextLex = stream.front();
        if (nextLex.getType() == FILEEND) //If file end token, ends stream
            done = true;
        stream.pop();
        return nextLex;
    }

    ///Getter
    bool isOpen(){
        return open;
    }

    ///Getter
    bool isDone(){
        return done;
    }
};

} //Namespace Lexer end

#endif //LEXEMESTREAM_H