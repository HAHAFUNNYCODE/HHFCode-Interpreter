#include <string>
#include <iostream>

#include "lexemes.h"
#include "lexemestream.h"
#include "lexer.h"

class LexerUninitializedException : std::exception{ //Thrown if the lexer is used without initialization
    const char* what () const throw() {
        return "The Lexer has not been initialized.";
    }
};

void Lexer::initialize(){ //Sets up values and loads patterns into Tries
    initializeLexemes();
    loadPatterns();
    initialized = true;
}

//Gets all tokens and puts them in a stream. 
//Will throw exception if not initialized
LexemeStream Lexer::tokenize(std::string &input){ 
    if(!initialized)
        throw LexerUninitializedException();

    LexemeStream lexstream;

    IndexTracker index; //uses index to store where the lexer is in the string

    while(index < input.size()){ //Continues until it reaches the end of the input
        char curC = input[index]; //looks at current character
        if(!curC)
            break;
        //Check for Whitespace
        if(whitespace.find(curC) != whitespace.end()){ //skips if whitespace
            index++;
            if(curC == '\n')
                index.addLine(1);
            else
                index.addCol(1);
            continue;
        }

        //Check for comments
        //If it is possible that this is a comment, looks through notation
        if(startChars[COMMENT].find(curC) != startChars[COMMENT].end()){
            Lexeme lex = getComment(input, &index); //Runs specific function
            Token type = lex.getType(); //Checks token validity
            if (type == INVALID)
                throw InvalidTokenException();
            if (type == COMMENT){ //Checks if correct token
                lexstream.pushLexeme(lex);
                continue;
            }
        }

        //Check for keywords
        if(startChars[KEYWORD].find(curC) != startChars[KEYWORD].end()){
            Lexeme lex = getFromPattern(input, &index, KEYWORD); //General pattern search
            Token type = lex.getType(); //Checks token validity
            if (type == INVALID)
                throw InvalidTokenException();
            if (type == KEYWORD){ //Checks if correct token
                lexstream.pushLexeme(lex);
                index.addCol(lex.getValue().size());
                continue;
            }
        }

        //Check for literals
        if(startChars[LITERAL].find(curC) != startChars[LITERAL].end()){
            Lexeme lex = getLiteral(input, &index); //Specific function
            Token type = lex.getType(); //Checks token validity
            if (type == INVALID)
                throw InvalidTokenException();
            if (type == LITERAL){ //Checks if correct token
                lexstream.pushLexeme(lex);
                continue;
            }
        }

        //Check for operators
        if(startChars[OPERATOR].find(curC) != startChars[OPERATOR].end()){
            Lexeme lex = getFromPattern(input, &index, OPERATOR, false); //General search
            Token type = lex.getType(); //Checks token validity
            if (type == INVALID)
                throw InvalidTokenException();
            if (type == OPERATOR){ //Checks if correct token
                lexstream.pushLexeme(lex);
                index.addCol(lex.getValue().size());
                continue;
            }
        }

        //Check for separators
        //Separators are all one character in this, therefore simply adds lexeme with no need to check
        if(startChars[SEPARATOR].find(curC) != startChars[SEPARATOR].end()){
            std::string charS(1,curC);
            Lexeme lex(SEPARATOR, charS, index.line, index.col);
            lexstream.pushLexeme(lex);
            index++;
            index.addCol(1);
            continue;
        }

        //Check for Identifiers
        //Checks to make sure that the first character is a non-digit letter or underscore.
        if(identifierSet.find(curC) != identifierSet.end() && !('0' <= curC && curC <= '9')){
            Lexeme lex = getIdentifier(input, &index); //Specific function
            Token type = lex.getType(); //Checks token validity
            if (type == INVALID)
                throw InvalidTokenException();
            if (type == IDENTIFIER){ //Checks if correct token
                lexstream.pushLexeme(lex);
                index.addCol(lex.getValue().size());
                continue;
            }
        }

        //If it gets here, either the interpreter is bad or the program is bad.
        throw InvalidTokenException("Nothing matched to language tokens. (Line: "
         + std::to_string(index.line) + ", Column: " + std::to_string(index.col) + ") Character: " + std::to_string(int(curC)));

    } 


    lexstream.finish(); //Sends EOF to stream
    return lexstream;
}


void Lexer::loadPatterns(){ //Takes all of the loaded lexeme symbols and puts them in Tries.
    for(auto symbols : knownSymbols){
        Trie t;
        for(std::string symbol : symbols.second){
            t.addPattern(symbol);
        }
        patterns[symbols.first] = t;
    }
}

std::string prettifyComment(std::string);

Lexeme Lexer::getComment(std::string& input, IndexTracker* index){
    IndexTracker indexfallback = *index; //In case need to go back to start
    std::shared_ptr<Trie> node = patterns[COMMENT].getRef(input[(*index)++]), prev; //Starts Trie
    index->addCol(1);
    std::string value = "";
    char cur;

    //Searches for start pattern using try. If it stops matching then node will be nullptr (false)
    while (node && *index < input.size()){
        value += node->getValue(); //Adds to value to hold the current match
        prev = node;
        cur = input[(*index)++];
        node = node->getRef(cur);
        if(cur == '\n') 
            index->addLine(1);
        else
            index->addCol(1);
    }

    std::string comment(1,cur); //Starts comment content

    if(value == lineCommentStart){ //If the start matches line comment
        while(*index < input.size()){ //Gets all characters until next \n or EOF
            char cur = input[(*index)++];

            if(cur == '\n'){
                index->addLine(1);
                break;
            }
            comment += cur;
        }
        return Lexeme(COMMENT, comment, indexfallback.line, indexfallback.col);

    } else if(value == blockCommentStart){ //If matches block comment
        while(*index < input.size()){ //Gets characters until block comment end or EOF
            char cur = input[(*index)++];
            if (cur == '\n'){
                index->addLine(1);
                continue;
            }
            index->addCol(1);
            comment += cur;
            std::shared_ptr<Trie> endNode = patterns[COMMENTEND].getRef(cur); //Tries to start Trie search
            
            while(*index < input.size() && endNode && !endNode->isEnding()){ // If the trie starts to match, see if full match
                cur = input[(*index)++];
                comment += cur;
                endNode = endNode->getRef(cur);
                if(cur == '\n')
                    index->addLine(1);
                else
                    index->addCol(1);
            }

            if(endNode && endNode->isEnding()){ //If full match, then found the end of the comment
                comment.resize(comment.size() - blockCommentEnd.size());
                size_t nlPos = comment.find('\n');
                return Lexeme(COMMENT, prettifyComment(comment), indexfallback.line, indexfallback.col);
            }
        }
        //Comment blocks extending past EOF are invalid
        return Lexeme(INVALID);
    }

    //If starts to match to comment start but fails
    *index = indexfallback;
    return Lexeme();
}

Lexeme Lexer::getFromPattern(std::string& input, IndexTracker* index, Token pat, bool needSep){
    IndexTracker indexfallback = *index;
    std::shared_ptr<Trie> node = patterns[pat].getRef(input[(*index)++]), prev;
    std::string value = "";

    while (node && *index < input.size()){ //Does try serach
        value += node->getValue();
        prev = node;
        char cur = input[(*index)++];
        node = node->getRef(cur);
    }

    if(*index < input.size()){ //If not EOF yet
        (*index)--;
        if (prev && prev->isEnding()){ //If has a match, check last character
            if(!needSep){ //If last character doesn't matter, then just return
                return Lexeme(pat, value, indexfallback.line, indexfallback.col);
            }
            
            //Checks last character for whitespace or separator
            if( (whitespace.find(input[*index]) != whitespace.end() ) || 
                startChars[SEPARATOR].find(input[*index]) != startChars[SEPARATOR].end()){
                    return Lexeme(pat, value, indexfallback.line, indexfallback.col);
            } 

        }
    }

    else{ //If EOF
        if(node && node->isEnding()){
            return Lexeme(pat, value + node->getValue(), indexfallback.line, indexfallback.col);
        }

        if(!node && prev->isEnding()){
            return Lexeme(pat, value, indexfallback.line, indexfallback.col);
        }
    }

    //If fails, go back to start of match
    *index = indexfallback;
    return Lexeme();
}

Lexeme Lexer::getLiteral(std::string& input, IndexTracker* index){
    //Wrapper for different literal types
    char start = input[*index];
    if(start == '"'){ //For string
        return getString(input, index);
    } else if(start == '.' || ('0' <= start && start <= '9') || start == '-'){ //For int/float
        return getNumber(input, index);
    }

    //for word based literals like booleans
    return getFromPattern(input, index, LITERAL);
}

Lexeme Lexer::getString(std::string& input, IndexTracker* index){
    IndexTracker indexfallback = *index;
    std::string str(1,input[(*index)++]);
    bool foundEnd = false;
    while(!foundEnd && *index < input.size()){ //Looks for a non-escaped " char
        if(input[*index] == '\n'){
            (*index)++;
            index->addLine(1);
            continue;
        }
        index->addCol(1);
        if(input[*index] == '"' && input[(*index) - 1] != escapeChar){
            foundEnd = true;
        }
        str += input[(*index)++];
    }

    if(foundEnd) //If found, then that is the literal
        return Lexeme(LITERAL, str, indexfallback.line, indexfallback.col);

    if(*index >= input.size()){ //If reached EOF, throws error
        *index = indexfallback;
        return Lexeme(INVALID, "", indexfallback.line, indexfallback.col);
    }

    return Lexeme(INVALID, "", indexfallback.line, indexfallback.col);
}

Lexeme Lexer::getNumber(std::string& input, IndexTracker* index){
    IndexTracker indexfallback = *index;
    std::string number;
    bool passedPeriod = false; //Number can only have one period

    if(input[*index] == '-'){ //Checks for possible negative
        number += '-';
        (*index)++;
    }
    
    while(*index < input.size()){ //while not at EOF, adds to number
        char cur = input[*index];
        if('0' <= cur && cur <= '9'){
            number += cur;
            (*index)++;
            index->addCol(1);
            continue;
        }

        if(cur == '.'){ //Checks for period, if one has passed, it's invalid
            if(!passedPeriod){
                number += '.';
                (*index)++;
                index->addCol(1);
                passedPeriod = true;
                continue;
            } else{
                return Lexeme(INVALID, "", indexfallback.line, indexfallback.col);
            }
        }

        //Whitespace or Separator means stop
        if(whitespace.find(cur) != whitespace.end() || startChars[SEPARATOR].find(cur) != startChars[SEPARATOR].end()){
            return Lexeme(LITERAL, number, indexfallback.line, indexfallback.col);
        }

        //Any other character is invalid
        return Lexeme(INVALID, "", indexfallback.line, indexfallback.col);
    }
    
    //If EOF, then make literal
    if(*index == input.size())
        return Lexeme(LITERAL, number, indexfallback.line, indexfallback.col);

    *index = indexfallback;
    return Lexeme(INVALID, "", indexfallback.line, indexfallback.col);
}

//Looks for a string of valid identifier characters followed by whitespace, separator, or operator.
Lexeme Lexer::getIdentifier(std::string& input, IndexTracker* index){
    IndexTracker indexfallback = *index;
    std::string value(1,input[(*index)++]);

    while(*index < input.size() && identifierSet.find(input[*index]) != identifierSet.end()){
        value += input[(*index)++];
    }

    if(*index >= input.size()){
        return Lexeme(IDENTIFIER, value, indexfallback.line, indexfallback.col);
    }

    if(whitespace.find(input[*index]) != whitespace.end() ||
       startChars[SEPARATOR].find(input[*index]) != startChars[SEPARATOR].end() ||
       startChars[OPERATOR].find(input[*index]) != startChars[OPERATOR].end() ){
           return Lexeme(IDENTIFIER, value, indexfallback.line, indexfallback.col);
    }

    *index = indexfallback;
    return Lexeme(UNKNOWN, "", -1, -1);
}

std::string prettifyComment(std::string str){
    size_t nlPos;

    while((nlPos = str.find('\n')) != std::string::npos){
        str.replace(nlPos, size_t(1), "");
    }

    while((nlPos = str.find("  ")) != std::string::npos){
        str.replace(nlPos, size_t(2), " ");
    }
    return str;
}