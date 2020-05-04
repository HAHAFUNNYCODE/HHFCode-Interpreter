#ifndef MAIN_H
#define MAIN_H

#include "lexer/lexemes.h"
#include "lexer/lexemestream.h"

extern std::unordered_map<Token, std::vector<std::string>, TokenHash> knownSymbols;

#endif //MAIN_H