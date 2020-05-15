#include <string>
#include <array>
#include <memory>

#include "lexeme.h"
#include "lexemestream.h"
#include "lexer.h"
#include "indexTracker.h"

namespace Lexer{

void Lexer::loadPatterns(){ //Takes all of the loaded lexeme symbols and puts them in Tries.
    for(auto symbols : knownSymbols){
        Trie t;
        for(std::string symbol : symbols.second){
            t.addPattern(symbol);
        }
        patterns[symbols.first] = t;
    }
}

//Gets all tokens and puts them in a stream. 
//Will throw exception if not initialized
void Lexer::tokenize(LexemeStream& stream, std::string &input){ 
    if(!initialized)
        throw LexerUninitializedException();

    IndexTracker index; //uses index to store where the lexer is in the string

    // std::array<Token, 6> tokenPriority = {COMMENT, KEYWORD, LITERAL, OPERATOR, SEPARATOR, IDENTIFIER};

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

        if(startChars[COMMENT].find(curC) != startChars[COMMENT].end()){
            Lexeme lex = getComment(input, &index);
            if(validateLexeme(COMMENT, lex)){
                stream.pushLexeme(lex);
                continue;
            }
        }

        if(startChars[KEYWORD].find(curC) != startChars[KEYWORD].end()){
            Lexeme lex = getKeyword(input, &index);
            if(validateLexeme(KEYWORD, lex)){
                stream.pushLexeme(lex);
                continue;
            }
        }

        if(startChars[LITERAL].find(curC) != startChars[LITERAL].end()){
            Lexeme lex = getLiteral(input, &index);
            if(validateLexeme(LITERAL, lex)){
                stream.pushLexeme(lex);
                continue;
            }
        }

        if(startChars[OPERATOR].find(curC) != startChars[OPERATOR].end()){
            Lexeme lex = getOperator(input, &index);
            if(validateLexeme(OPERATOR, lex)){
                stream.pushLexeme(lex);
                continue;
            }
        }

        if(startChars[SEPARATOR].find(curC) != startChars[SEPARATOR].end()){
            Lexeme lex = getSeparator(input, &index);
            if(validateLexeme(SEPARATOR, lex)){
                stream.pushLexeme(lex);
                continue;
            }
        }

        if(startChars[IDENTIFIER].find(curC) != startChars[IDENTIFIER].end()){
            Lexeme lex = getIdentifier(input, &index);
            if(validateLexeme(IDENTIFIER, lex)){
                stream.pushLexeme(lex);
                continue;
            }
        }

        //If it gets here, either the interpreter is bad or the program is bad.
        throw Lexeme::InvalidTokenException("Nothing matched to language tokens. (Line: "
         + std::to_string(index.line) + ", Column: " + std::to_string(index.col) + ") Character: " + std::to_string(int(curC)));

    } 


    stream.finish(); //Sends EOF to stream
}

bool Lexer::validateLexeme(Token type, Lexeme& lex){
    Token lexType = lex.getType(); //Checks token validity
    if (lexType == type){ //Checks if correct token
        return true;
    }
    if (lexType == INVALID)
        throw Lexeme::InvalidTokenException();

    return false;
}


/**
 * @brief Tries to get a comment.
 * @details It looks through the start of the string section and tries to match to either a
 *  line or block comment start. If it finds a line comment, it will continue to search until
 *  it finds a endline character or EOF. If it finds a block, it will match until closing sequence.
 * 
 * @param input A reference to the input data string.
 * @param index A pointer to the index keeping track of file position.
 * @returns COMMENT lexeme if a valid comment, UNKNOWN lexeme if starting sequence doesn't match,
 *  INVALID lexeme if block comment hits EOF.
 * */
Lexeme Lexer::getComment(std::string& input, IndexTracker* index){
    IndexTracker indexfallback = *index; //In case need to go back to start
    std::shared_ptr<Trie> node = patterns[COMMENT].getRef(input[(*index)++]), prev; //Starts Trie
    index->addCol(1);
    std::string value = "";
    char cur = 0;

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
                return Lexeme(COMMENT, prettifyComment(comment), indexfallback.line, indexfallback.col);
            }
        }
        //Comment blocks extending past EOF are invalid
        return Lexeme(INVALID, "", indexfallback.line, indexfallback.col);
    }

    //If starts to match to comment start but fails
    *index = indexfallback;
    return Lexeme();
}

///Returns a language keyword from pattern.
Lexeme Lexer::getKeyword(std::string& input, IndexTracker* index){
    return getFromPattern(input, index, KEYWORD);
}

///Tries to get a literal
Lexeme Lexer::getLiteral(std::string& input, IndexTracker* index){
    //Wrapper for different literal types
    char start = input[*index];
    if(start == '"'){ //For string
        return getString(input, index);
    } else if(start == '.' || ('0' <= start && start <= '9') || start == '-'){ //For int/float
        return getNumber(input, index);
    }

    //for word based literals like booleans
    Lexeme lex = getFromPattern(input, index, LITERAL);
    return lex;
}

/**
 * @brief Tries to get a string.
 * @details Starting from a " character, it looks for the next " character, skipping newlines.
 * 
 * @param input A reference to the input data string.
 * @param index A pointer to the index keeping track of file position.
 * @returns A LITERAL lexeme if it finds a second " character or INVALID if EOF.
 * */
Lexeme Lexer::getString(std::string& input, IndexTracker* index){
    IndexTracker indexfallback = *index;
    std::string str(1,input[(*index)++]);
    index->addCol(1);
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

/**
 * @brief Tries to get a number.
 * @details Looks for either an integer or a floating point value. It stops when it hits an
 *  operator, separator, or whitespace. It allows for numbers containing only digits,
 *  containing a single '.' character in between, or starting with or ending with a '.' character.
 * 
 * @param input A reference to the input data string.
 * @param index A pointer to the index keeping track of file position.
 * @returns A LITERAL lexeme if it finds a valid number character or EOF or INVALID if it finds
 *  a non-terminating character or multiple decimal points.
 * */
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

///Returns operator based on pattern
Lexeme Lexer::getOperator(std::string& input, IndexTracker* index){
    return getFromPattern(input, index, OPERATOR, false);
}

///Returns single character separator
Lexeme Lexer::getSeparator(std::string& input, IndexTracker* index){
    std::string charS(1,input[(*index)++]);
    Lexeme lex(SEPARATOR, charS, index->line, index->col);
    index->addCol(1);
    return lex;
}

///Tries to get an identifier
Lexeme Lexer::getIdentifier(std::string& input, IndexTracker* index){
    IndexTracker indexfallback = *index;
    
    char c = input[*index];
    if('0' <= c && c <= '9') //Identifiers can't start with a digit
        return Lexeme();

    std::string value(1,c);
    (*index)++;

    while(*index < input.size() && identifierSet.find(input[*index]) != identifierSet.end()){
        value += input[(*index)++];
    }

    if(*index >= input.size()){
        index->addCol(value.size());
        return Lexeme(IDENTIFIER, value, indexfallback.line, indexfallback.col);
    }

    if(whitespace.find(input[*index]) != whitespace.end() ||
       startChars[SEPARATOR].find(input[*index]) != startChars[SEPARATOR].end() ||
       startChars[OPERATOR].find(input[*index]) != startChars[OPERATOR].end() ){
           index->addCol(value.size());
           return Lexeme(IDENTIFIER, value, indexfallback.line, indexfallback.col);
    }

    *index = indexfallback;
    return Lexeme();
}

/**
 * @brief Tries to get a pattern.
 * @details It looks through the start of the string section and tries to match with any pattern
 *  in the corresponding Trie. It continues until an invalid character is found or EOF.
 * 
 * @param input A reference to the input data string.
 * @param index A pointer to the index keeping track of file position.
 * @param pat The token representing which pattern to match
 * @param needSep A flag to tell the function if a separator or whitespace is needed following
 *  the pattern. Default true.
 * @returns lexeme type specific in "pat" or UNKNOWN if it doesn't find a pattern.
 * */
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
                index->addCol(value.size());
                return Lexeme(pat, value, indexfallback.line, indexfallback.col);
            }
            
            //Checks last character for whitespace or separator
            if( (whitespace.find(input[*index]) != whitespace.end() ) || 
                startChars[SEPARATOR].find(input[*index]) != startChars[SEPARATOR].end()){
                    index->addCol(value.size());
                    return Lexeme(pat, value, indexfallback.line, indexfallback.col);
            } 

        }
    }

    else{ //If EOF
        if(node && node->isEnding()){
            index->addCol(value.size());
            return Lexeme(pat, value + node->getValue(), indexfallback.line, indexfallback.col);
        }

        if(!node && prev->isEnding()){
            index->addCol(value.size());
            return Lexeme(pat, value, indexfallback.line, indexfallback.col);
        }
    }

    //If fails, go back to start of match
    *index = indexfallback;
    return Lexeme();
}

/**
 * @brief Makes a string more pretty.
 * @details Takes in a comment and strips away repeated whitespace and any newlines
 *  found in the string.
 * @param str The input comment to be prettified
 * @returns The prettified comment
 * */
std::string Lexer::prettifyComment(std::string str){
    size_t nlPos;

    while((nlPos = str.find('\n')) != std::string::npos){
        str.replace(nlPos, size_t(1), "");
    }

    while((nlPos = str.find("  ")) != std::string::npos){
        str.replace(nlPos, size_t(2), " ");
    }
    return str;
}

} //Namespace Lexer end