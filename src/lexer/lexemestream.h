#ifndef LEXEMESTREAM_H
#define LEXEMESTREAM_H

#include <queue>
#include <exception>

namespace Lexer{

class Lexeme;

class ClosedLexStreamException : std::exception { //When trying to push to closed stream
    const char* what () const throw() {
        return "Cannot update closed stream.";
    }
};

class FinishedLexStreamException : std::exception { //When trying to access values from stream with no values
    const char* what () const throw() {
        return "Tried to access value from ended stream";
    }
};


class LexemeStream { //Stream responsible for outputing lexemes.
    private:
    std::queue<Lexeme> stream;
    bool open, done;

    public:
    LexemeStream(): open(true), done(false){}

    void pushLexeme(Lexeme lex){ //Adds to stream
        if (open) //If the stream is available to be pushed to, do so
            stream.push(lex);
        else //Otherwise throw excpetion
            throw ClosedLexStreamException();
    }

    void finish(){ //Pushes an EOF lexeme and closed input of stream
        pushLexeme(
            Lexeme(FILEEND, "EOF", -1, -1)
        );
        open = false;
    }

    Lexeme next(){ //Returns the next lexeme in the stream.
        if (done || stream.empty()) //If finished then throws exception
            throw FinishedLexStreamException();

        Lexeme nextLex = stream.front();
        if (nextLex.getType() == FILEEND) //If file end token, ends stream
            done = true;
        stream.pop();
        return nextLex;
    }

    bool isOpen(){
        return open;
    }

    bool isDone(){
        return done;
    }
};

} //Namespace Lexer end

#endif //LEXEMESTREAM_H