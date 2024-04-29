#include <iostream>

#include "DSString.h"
#include "SentimentAnalyzer.h"
#include <chrono>

int main(int argc, char** argv)
{  
    // Argument format:
    // $ executable trainingInputFile testingInputFile groundTruthInputFile resultsOutputFile accuracyOutputFile
    if (argc != 6) {
        std::cerr << "Error: " << argc << " arguments given. 5 expected.\n";
        return 1;
    }

    SentimentAnalyzer analyzer;
    bool success = true;

    std::cout << "Training...\n";
    success = analyzer.Train(argv[1]);
    if (!success) return 1;
    
    std::cout << "Testing...\n";
    success = analyzer.Test(argv[2], argv[4]);
    if (!success) return 1;

    std::cout << "Evaluating...\n";
    success = analyzer.Evaluate(argv[3], argv[5]);
    if (!success) return 1; else {
        std::cout << "Done!\n";
    }

    return 0;
}