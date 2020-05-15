#include <iostream>
#include <fstream>

#include "main.h"

std::string openFile(std::string);

int main(int argc, char* argv[]){ //Takes in command line arguments for the file which to open.
    Lexer::Lexer lex = Lexer::BasicLexer();
    Lexer::LexemeStream lexstream;

    //Gets the input from a specified file
    std::string inputStr;
    if (argc == 2){
        std::string fname = std::string(argv[1]);
        inputStr = openFile(fname);
    }

    std::vector <double> times;

    Util::Timer timer(Util::Timer::MILLI); //For testing really, starts a timer
    for(int i = 0; i < 10000; i++){
        Lexer::LexemeStream stream;
        timer.start();
        try{ //Tries to have the data from the file tokenized. If it fails, it will raise
            // an InvalidTokenException which can be read
            lex.tokenize(stream, inputStr);
        }catch(Lexer::Lexeme::InvalidTokenException& e){
            std::cout << "\e[91mERROR: \e[0m" << e.what() << std::endl;
            throw Lexer::Lexeme::InvalidTokenException(e);
        }
        timer.stop();
        times.push_back(timer.getDuration());
        lexstream = stream;
    }

    double min, max, avg;
    Util::MinMaxAvg(times, &min, &max, &avg);

    while (!lexstream.isDone()) //Continues while the stream still has something inside of it
    {
        std::cout << lexstream.next().getString() << std::endl;
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