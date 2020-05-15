#ifndef INDEXTRACKER_H
#define INDEXTRACKER_H

#include <cstddef>

namespace Lexer{

///A position tracker for the file.
struct IndexTracker{
    size_t index, line, col;

    ///Default constructor
    IndexTracker():index(0),line(1),col(1){}

    ///Value Constructor
    IndexTracker(size_t index, size_t line, size_t col): 
        index(index), line(line), col(col){}

    ///Copy Constructor
    IndexTracker(const IndexTracker& other):
        index(other.index), line(other.line), col(other.col){}

    ///Goes to next line
    void addLine(size_t n){
        line += n;
        col = 1;
    }

    ///Goes to next column
    void addCol(size_t n){
        col += n;
    }

    ///Increases index by 1
    size_t operator++(int n __attribute__((unused))){
        index+=1;
        return index-1;
    }

    ///Decreases index by 1
    size_t operator--(int n __attribute__((unused))){
        index-=1;
        return index+1;
    }

    ///Conversion to size_t
    inline operator size_t(){
        return index;
    }
};

} //Namespace Lexer end


#endif //INDEXTRACKER_H