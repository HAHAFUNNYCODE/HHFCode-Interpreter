#include "trie.h"

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