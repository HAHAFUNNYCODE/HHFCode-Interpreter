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

    size_t index = 0; //uses index to store where the lexer is in the string

    while(index < input.size()){ //Continues until it reaches the end of the input
        char curC = input[index]; //looks at current character
        //Check for Whitespace
        if(whitespace.find(curC) != whitespace.end()){ //skips if whitespace
            index++;
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
                continue;
            }
        }

        //Check for separators
        //Separators are all one character in this, therefore simply adds lexeme with no need to check
        if(startChars[SEPARATOR].find(curC) != startChars[SEPARATOR].end()){
            std::string charS(1,curC);
            Lexeme lex(SEPARATOR, charS, -1,-1);
            lexstream.pushLexeme(lex);
            index++;
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
                continue;
            }
        }

        //If it gets here, either the interpreter is bad or the program is bad.
        throw InvalidTokenException("Nothing matched to language tokens.");

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

void Trie::addPattern(std::string p){ //Calls the recursive one
    addPattern(p,0);
}

void Trie::addPattern(std::string p, int index){ //If node exists, adds to node, otherwise makes new node
    if (index > 0){ //Sets values for node (should this check if value exists? might be not worth it)
        value = p[index-1];
        //The reason this works is that the node only switches below
    }
    if(index >= p.size()){ //Sets the end of a pattern
        isEnd = true; //This will work because the node switches below, therefore will always be valid node
        return;
    }
    char c = p[index];

    if(charset.find(c) == charset.end()){ //Checks if current char is already a node, if not adds it.
        charset.insert(c);
        refs[c] = std::make_shared<Trie>();
    }
    refs[c]->addPattern(p, index+1); //Goes to next node
}


//Do Later, not sure if needed
bool Trie::patternExists(std::string pat){
    return false;
}

//Gets the pointer to a node if it is linked to the current one, otherwise nullptr
std::shared_ptr<Trie> Trie::getRef(char c){
    if (charset.find(c) == charset.end())
        return std::shared_ptr<Trie>(nullptr);
    else
        return refs[c];
}

// void Trie::print(){
//     for(char c : charset){
//         std::string s(1, c);
//         refs[c]->print(s);
//     }
// }

// void Trie::print(std::string s){
//     if(charset.empty()){
//         std::cout << s << std::endl;
//         return;
//     }

//     if (isEnd)
//         std::cout << s << std::endl;

//     for(char c : charset){
//         refs[c]->print(s+c);
//     }
// }

Lexeme Lexer::getComment(std::string& input, size_t* index){
    size_t indexfallback = *index; //In case need to go back to start
    std::shared_ptr<Trie> node = patterns[COMMENT].getRef(input[(*index)++]), prev; //Starts Trie
    std::string value = "";
    char cur;

    //Searches for start pattern using try. If it stops matching then node will be nullptr (false)
    while (node && *index < input.size()){
        value += node->getValue(); //Adds to value to hold the current match
        prev = node;
        cur = input[(*index)++];
        node = node->getRef(cur);
    }

    std::string comment(1,cur); //Starts comment content

    if(value == lineCommentStart){ //If the start matches line comment
        while(*index < input.size()){ //Gets all characters until next \n or EOF
            char cur = input[(*index)++];
            if(cur == '\n'){
                (*index)++;
                break;
            }
            comment += cur;
        }
        return Lexeme(COMMENT, comment, -1,-1); 
    } else if(value == blockCommentStart){ //If matches block comment
        while(*index < input.size()){ //Gets characters until block comment end or EOF
            char cur = input[(*index)++];
            comment += cur;
            std::shared_ptr<Trie> endNode = patterns[COMMENTEND].getRef(cur); //Tries to start Trie search
            
            while(*index < input.size() && endNode && !endNode->isEnding()){ // If the trie starts to match, see if full match
                cur = input[(*index)++];
                comment += cur;
                endNode = endNode->getRef(cur);
            }

            if(endNode && endNode->isEnding()){ //If full match, then found the end of the comment
                comment.resize(comment.size() - blockCommentEnd.size());
                return Lexeme(COMMENT, comment, -1, -1);
            }
        }
        //Comment blocks extending past EOF are invalid
        return Lexeme(INVALID);
    }

    //If starts to match to comment start but fails
    *index = indexfallback;
    return Lexeme();
}

Lexeme Lexer::getFromPattern(std::string& input, size_t* index, Token pat, bool needSep){
    size_t indexfallback = *index;
    std::shared_ptr<Trie> node = patterns[pat].getRef(input[(*index)++]), prev;
    std::string value = "";

    while (node && *index < input.size()){ //Does try serach
        value += node->getValue();
        prev = node;
        node = node->getRef(input[(*index)++]);
    }

    if(*index < input.size()){ //If not EOF yet
        (*index)--;
        if (prev && prev->isEnding()){ //If has a match, check last character
            if(!needSep){ //If last character doesn't matter, then just return
                return Lexeme(pat, value, -1, -1);
            }
            
            //Checks last character for whitespace or separator
            if( (whitespace.find(input[*index]) != whitespace.end() ) || 
                startChars[SEPARATOR].find(input[*index]) != startChars[SEPARATOR].end()){
                    return Lexeme(pat, value, -1, -1);
            } 

        }
    }

    else{ //If EOF
        if(node && node->isEnding()){
            return Lexeme(pat, value + node->getValue(), -1, -1);
        }

        if(!node && prev->isEnding()){
            return Lexeme(pat, value, -1, -1);
        }
    }

    //If fails, go back to start of match
    *index = indexfallback;
    return Lexeme();
}

Lexeme Lexer::getLiteral(std::string& input, size_t* index){
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

Lexeme Lexer::getString(std::string& input, size_t* index){
    size_t indexfallback = *index;
    std::string str(1,input[(*index)++]);
    bool foundEnd = false;
    while(!foundEnd && *index < input.size()){ //Looks for a non-escaped " char
        if(input[*index] == '"' && input[(*index) - 1] != escapeChar){
            foundEnd = true;
        }
        str += input[(*index)++];
    }

    if(foundEnd) //If found, then that is the literal
        return Lexeme(LITERAL, str, -1, -1);

    if(*index >= input.size()){ //If reached EOF, throws error
        *index = indexfallback;
        return Lexeme(INVALID);
    }

    return Lexeme(INVALID);
}

Lexeme Lexer::getNumber(std::string& input, size_t* index){
    size_t indexfallback = *index;
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
            continue;
        }

        if(cur == '.'){ //Checks for period, if one has passed, it's invalid
            if(!passedPeriod){
                number += '.';
                (*index)++;
                passedPeriod = true;
                continue;
            } else{
                return Lexeme(INVALID);
            }
        }

        //Whitespace or Separator means stop
        if(whitespace.find(cur) != whitespace.end() || startChars[SEPARATOR].find(cur) != startChars[SEPARATOR].end()){
            return Lexeme(LITERAL, number, -1, -1);
        }

        //Any other character is invalid
        return Lexeme(INVALID);
    }
    
    //If EOF, then make literal
    if(*index == input.size())
        return Lexeme(LITERAL, number, -1, -1);

    *index = indexfallback;
    return Lexeme(INVALID);
}

//Looks for a string of valid identifier characters followed by whitespace, separator, or operator.
Lexeme Lexer::getIdentifier(std::string& input, size_t* index){
    size_t indexfallback = *index;
    std::string value(1,input[(*index)++]);

    while(*index < input.size() && identifierSet.find(input[*index]) != identifierSet.end()){
        value += input[(*index)++];
    }

    if(*index >= input.size()){
        return Lexeme(IDENTIFIER, value, -1, -1);
    }

    if(whitespace.find(input[*index]) != whitespace.end() ||
       startChars[SEPARATOR].find(input[*index]) != startChars[SEPARATOR].end() ||
       startChars[OPERATOR].find(input[*index]) != startChars[OPERATOR].end() ){
           return Lexeme(IDENTIFIER, value, -1, -1);
    }

    *index = indexfallback;
    return Lexeme(UNKNOWN, "", -1, -1);
}