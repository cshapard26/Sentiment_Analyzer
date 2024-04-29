#include "SentimentAnalyzer.h"
#include "DSString.h"
#include "stemmer/Porter2Stemmer.h"
#include <fstream>
#include <cmath>
#include <iomanip>



SentimentAnalyzer::SentimentAnalyzer(){
    //Using NLTK's stop word list. Found at https://gist.github.com/sebleier/554280
    stopWords = {"", " ", "i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your", "yours", "yourself", "yourselves", "he", "him", "his", "himself", "she", "her", "hers", "herself", "it", "its", "itself", "they", "them", "their", "theirs", "themselves", "what", "which", "who", "whom", "this", "that", "these", "those", "am", "is", "are", "was", "were", "be", "been", "being", "have", "has", "had", "having", "do", "does", "did", "doing", "a", "an", "the", "and", "but", "if", "or", "because", "as", "until", "while", "of", "at", "by", "for", "with", "about", "against", "between", "into", "through", "during", "before", "after", "above", "below", "to", "from", "up", "down", "in", "out", "on", "off", "over", "under", "again", "further", "then", "once", "here", "there", "when", "where", "why", "how", "all", "any", "both", "each", "few", "more", "most", "other", "some", "such", "no", "nor", "not", "only", "own", "same", "so", "than", "too", "very", "s", "t", "can", "will", "just", "don", "should", "now", "QQ"};
    wordWeightDict.clear();
    testEvaluationDict.clear();
}


bool SentimentAnalyzer::Train(std::string trainingFile) {
    // Fine-tuning Variables. These are SPECIFIC TO A SET OF 20k TRAINING TWEETS
    int hotStopWordCap = 300; // How many occurances necessary to create a HotStopWord
    double hotDifference = hotStopWordCap * .28; // What percent variation in the results to justify a HotSwapWord
    double weightDampening = .4; // The power of the curve of the resulting weights

    // Base accuracy: 60.8%
    // Stop Words: 6.7% increase
    // HotStopWord: 2.5% increase
    // weightDampening: 1.5% increase
    // goodOfHumanity: 0.7% increase
    // Stemming Words: Negligible Impact

    // Open Training File
    std::ifstream trainingData(trainingFile);
    std::string rawLine;

    // Make sure file opens correctly
    try {
        getline(trainingData, rawLine);
        if (rawLine == "") {
            throw std::runtime_error("Error reading training file. Check filename and try again.\n");
        }
        if (std::count(rawLine.begin(), rawLine.end(), ',') != 5) {
            throw std::runtime_error("Error: Training file opened successfully, but it contains incorrect file formatting. Check that the Command Line arguments are in the correct order.\n");
        }
    } catch (std::runtime_error& err) {
        std::cerr << err.what();
        return false;
    }

    //Loop through each line
    while (getline(trainingData, rawLine)) {
        DSString dsLine = rawLine;
        std::vector<DSString> tokenizedLine = dsLine.tokenize(',');

        //tokenizedLine[5] is the line with the tweet
        //tokenizedLine[0] is sentiment
        //tokenizedLine[1] is ID
        
        // Assign sentiment based on records
        double lineSentiment = 0;
        if (tokenizedLine[0].DSToInt() == 0) {
            lineSentiment = -1.0;
        } else if (tokenizedLine[0].DSToInt() == 4) {
            lineSentiment = 1.0;
        }

        const std::vector<DSString> tweetWordList = tokenizedLine[5].tokenize(' ');
        for(size_t i = 0; i < tweetWordList.size(); i++){
            // For each word: clean it, stem it, add its sentiment to wordWeightDict, and check its HotStop status
            DSString currentWord = tweetWordList[i].cleanString();
            std::string toStemmer = currentWord.string();
            Porter2Stemmer::trim(toStemmer);
            Porter2Stemmer::stem(toStemmer);
            currentWord = toStemmer;
            if (!DSFind(stopWords, currentWord)) {
                wordWeightDict[currentWord] = lineSentiment + wordWeightDict[currentWord];
                hotStopWordDict[currentWord]++;
                if (hotStopWordDict[currentWord] >= hotStopWordCap && wordWeightDict[currentWord] <= hotDifference && wordWeightDict[currentWord] >= -1 * hotDifference) {
                    stopWords.push_back(currentWord);
                    wordWeightDict.erase(currentWord);
                }
            }  
        }
    }
    trainingData.close();

    // Dampen results based on a power curve
    std::map<DSString, int>::iterator it;
    for (auto const& [key, val] : wordWeightDict) {
        if (val != 0) {
            wordWeightDict[key] = ((val >= 0) ? 1 : -1) * pow(abs(val), weightDampening);
        }
    }
    return true;
}


bool SentimentAnalyzer::Test(std::string testingFile, std::string results) {
    std::ifstream testingData(testingFile);
    std::string rawLine;

    // Make sure file opens correctly
    try {
        getline(testingData, rawLine);
        if (rawLine == "") {
            throw std::runtime_error("Error reading testing file. Check filename and try again.\n");
        }
        if (std::count(rawLine.begin(), rawLine.end(), ',') != 4) {
            throw std::runtime_error("Error: Testing file opened successfully, but it contains incorrect file formatting. Check that the Command Line arguments are in the correct order.\n");
        }
    } catch (std::runtime_error& err) {
        std::cerr << err.what();
        std::ofstream resultsFile(results);
        resultsFile.close();
        return false;
    }

    // Loop through each line
    while (getline(testingData, rawLine)) {
        DSString dsLine = rawLine;
        std::vector<DSString> tokenizedLine = dsLine.tokenize(',');

        //tokenizedLine[4] is the line with the tweet
        //tokenizedLine[0] is ID

        const std::vector<DSString> tweetWordList = tokenizedLine[4].tokenize(' ');
        double currentSentiment = 0;
        for(size_t i = 0; i < tweetWordList.size(); i++){
            // For each word: clean it, stem it, and add its respective weight to the total tweet weight
            DSString currentWord = tweetWordList[i].cleanString();
            std::string toStemmer = currentWord.string();
            Porter2Stemmer::trim(toStemmer);
            Porter2Stemmer::stem(toStemmer);
            currentWord = toStemmer;
            if (!DSFind(stopWords, currentWord)) {
                currentSentiment += wordWeightDict[currentWord];
            }
        }
        // Map the total tweet weight to its ID
        testEvaluationDict[tokenizedLine[0]] = currentSentiment;
    }
    testingData.close();

    // Write evaluations to the results file
    std::ofstream resultsFile(results);
    for (auto const& [key, val] : testEvaluationDict) {
        resultsFile << (val < 0 ? 0 : 4) << ',' << key << '\n';
    }
    resultsFile.close();
    return true;
}

bool SentimentAnalyzer::Evaluate(std::string dataSentiments, std::string accuracy) {
    int total = 0;
    int totalCorrect = 0;
    double goodOfHumanity = 2.0; // The inherent good in humanity. Assumes near-neutral words are positive.
    std::ifstream actualData(dataSentiments);
    
    std::string rawLine;

    // Make sure file opens correctly
    try {
        getline(actualData, rawLine);
        if (rawLine == "") {
            throw std::runtime_error("Error reading Ground Truth file. Check filename and try again.\n");
        }
        if (std::count(rawLine.begin(), rawLine.end(), ',') != 1) {
            throw std::runtime_error("Error: Ground Truth file opened successfully, but it contains incorrect file formatting. Check that the Command Line arguments are in the correct order.\n");
        }
    } catch (std::runtime_error& err) {
        std::cerr << err.what();
        std::ofstream accuracyFile(accuracy);
        accuracyFile.close();
        return false;
    }
    
    // Loop through each line
    while (getline(actualData, rawLine)) {
        // For each tweet: add the ground truth to a dict, compare the actual and calculated sentiments, and update the total/totalCorrect tweets
        DSString dsLine = rawLine;
        std::vector<DSString> tokenizedLine = dsLine.tokenize(',');
        actualSentimentDict[tokenizedLine[1]] = tokenizedLine[0].DSToInt();
        testEvaluationDict[tokenizedLine[1]] += goodOfHumanity;
        if ((testEvaluationDict[tokenizedLine[1]] < 0 ? 0 : 4) == actualSentimentDict[tokenizedLine[1]]){
            totalCorrect++;
            testEvaluationDict.erase(tokenizedLine[1]);
        }
        total++;

    }
    actualData.close();

    // Write to accuracy file
    std::ofstream accuracyFile(accuracy);
    accuracyFile << std::fixed << std::setprecision(3) << (double) totalCorrect/total * 100 << "\n";
    for (auto const& [key, val] : testEvaluationDict) {
        accuracyFile << (val < 0 ? 0 : 4) << ',' << actualSentimentDict[key] << ',' << key << '\n';
    }
    accuracyFile.close();
    return true;
}

