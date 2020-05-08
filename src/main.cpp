#include <iostream>
#include <fstream>

#include "main.h"
#include "lexer/lexer.h"
#include "misc/testing.h"

std::string openFile(std::string);

int main(int argc, char* argv[]){ //Takes in command line arguments for the file which to open.
    Lexer lex;
    LexemeStream stream;
    lex.initialize(); //Sets up values for the lexer to use. Primarily allows lexemes to
    // set data and for tries to be generated.

    //Gets the input from a specified file
    std::string inputStr;
    if (argc == 2){
        std::string fname = std::string(argv[1]);
        inputStr = openFile(fname);
    }

    Timer timer(MILLI); //For testing really, starts a timer
    timer.start();
    for(int i = 0; i < 10000; i++){
        try{ //Tries to have the data from the file tokenized. If it fails, it will raise
            // an InvalidTokenException which can be read
            stream = lex.tokenize(inputStr);
        }catch(InvalidTokenException e){
            std::cout << "\e[91mERROR: \e[0m" << e.what() << std::endl;
            throw e;
        }
        timer.lap();
    }
    timer.stop();

    std::vector <double> times = timer.getLapTimes();
    times.pop_back();
    double min, max, avg;
    MinMaxAvg(times, &min, &max, &avg);

    while (!stream.isDone()) //Continues while the stream still has something inside of it
    {
        std::cout << stream.next().getString() << std::endl;
    }

    // std::cout << timer.getDuration() << "ms" << std::endl; //Prints out the time it took
    std::cout << "Min: " << min << "ms Max: " << max << "ms Avg: " << avg << "ms" << std::endl;
    
    return 0;
}

std::string openFile(std::string fname){
    std::string inputStr;
    std::ifstream inputFile(fname);
    inputFile.seekg(0,std::ios::end);
    inputStr.resize(inputFile.tellg());
    inputFile.seekg(0,std::ios::beg);
    inputFile.read(&inputStr[0], inputStr.size());
    inputFile.close();
    return inputStr;
}