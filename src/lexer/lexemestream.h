#ifndef LEXEMESTREAM_H
#define LEXEMESTREAM_H

#include <queue>
#include <exception>

#include "lexemes.h"

class ClosedLexStreamException;
class FinishedLexStreamException;

class ClosedLexStreamException : std::exception {
    const char* what () const throw() {
        return "Cannot update closed stream.";
    }
};

class FinishedLexStreamException : std::exception {
    const char* what () const throw() {
        return "Tried to access value from ended stream";
    }
};


class LexemeStream {
    private:
    std::queue<Lexeme> stream;
    bool open, done;

    public:
    LexemeStream(): open(true), done(false){}

    void pushLexeme(Lexeme lex){
        if (open) //If the stream is available to be pushed to, do so
            stream.push(lex);
        else //Otherwise throw excpetion
            throw ClosedLexStreamException();
    }

    void finish(){ //Pushes an EOF lexeme and closed input of stream
        pushLexeme(
            Lexeme(FILEEND, "EOF", 0, stream.size() + 1)
        );
        open = false;
    }

    Lexeme next(){ //Returns the next lexeme in the stream.
        if (done)
            throw FinishedLexStreamException();

        Lexeme nextLex = stream.front();
        if (nextLex.getType() == FILEEND)
            done = true;
        stream.pop();
        return nextLex;
    }
};

#endif //LEXEMESTREAM_H