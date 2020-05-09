#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <string>

class Trie{ //A node for a Trie data structure
    private:
    char value; //The value of the node
    std::unordered_set<char> charset; //The characters that a node connects to
    std::unordered_map<char, std::shared_ptr<Trie> > refs; //Links to other nodes
    bool isEnd; //Flag for if a pattern ends at this node
    
    public:
    Trie(){}
    
    void addPattern(std::string); //Adds a pattern
    bool patternExists(std::string); //Checks if pattern exists (currently not implemented because wasn't needed)
    std::shared_ptr<Trie> getRef(char c); //Gets the node from the link of the character or nullptr if no node
    inline bool isEnding() {return isEnd;}
    inline bool isBottom() {return charset.empty();}
    inline char getValue() {return value;}
    
    // void print();

    private:
    void addPattern(std::string, size_t); //Recursive add pattern
    void print(std::string);
};