#include "trie.h"

namespace Lexer{

///Adds the pattern to the Trie
void Trie::addPattern(std::string p){ //Calls the private recursive one
    addPattern(p,0);
}

void Trie::addPattern(std::string p, size_t index){ //If node exists, adds to node, otherwise makes new node
    if (index > 0){ //Sets values for node (should this check if value exists? might be not worth it)
        value = p[index-1];
        //The reason this works is that the node only switches below
    }
    if(index >= p.size()){ //Sets the end of a pattern
        isEnd = true; //This will work because the node switches below, therefore will always be valid node
        return;
    }
    char c = p[index];

    if(!refs[c]){ //Checks if current char is already a node, if not adds it.
        refs[c] = std::make_shared<Trie>();
    }
    refs[c]->addPattern(p, index+1); //Goes to next node
}


//Do Later, not sure if needed
// bool Trie::patternExists(std::string pat){
//     return false;
// }

///Gets a pointer to next node with given character value.
std::shared_ptr<Trie> Trie::getRef(char c){
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

} //Namespace Lexer end