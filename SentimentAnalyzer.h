#ifndef SENTIMENT_ANALYZER_H
#define SENTIMENT_ANALYZER_H

#include <iostream>
#include <map>
#include "DSString.h"

class SentimentAnalyzer {
    private:
        std::map<DSString, double> wordWeightDict; // A map/dictionary of all the words evaluated from the training set assigned to thier respective weights
        std::vector<DSString> stopWords; // A vector of initial Stop Words
        std::map<DSString, double> testEvaluationDict; // A map/dictionary of the testing tweet IDs assigned to their evaluated truth
        std::map<DSString, int> actualSentimentDict; // A map/dictionary of all of the testing tweet IDs assigned to their Ground Truth
        std::map<DSString, int> hotStopWordDict; // A map/dictionary of all the words assigned to their number of occurances across all tweets.

    public:
        SentimentAnalyzer();

        /**
         * @brief Loops through a file containing training tweets and builds up the class' wordWeightDict and hotStopWordDict
         *
         * @return bool (ran successfully)
         */
        bool Train(std::string);

        /**
         * @brief Loops through a file containing testing tweets and evaluates each tweet, assigning it's evaluated weight to 
         *        the class' testEvaluationDict. Also exports a file mapping the tweets to their evaluated sentiment in a standard format.
         * 
         * @return bool (ran successfully)
         */
        bool Test(std::string, std::string);

        /**
         * @brief Loops through a file containing the ground truth of the testing tweets and adds each one to 
         *        the class' actualSentimentDict. Also exports a file with the total accuracy and a list of the incorrectly evaluated tweets.
         *
         * @return bool (ran successfully)
         */
        bool Evaluate(std::string, std::string);

};

#endif