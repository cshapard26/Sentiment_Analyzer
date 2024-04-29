# Assignment 2: Answers

**Complete this document, commit your changes to Github and submit the repository URL to Canvas.** Keep your answers short and precise.

Your Name: Cooper Shapard

Used free extension: [ ] 24 hrs or [ ] 48 hrs

[x] Early submission (48 hrs)

[x] Bonus work. Describe: >72.000% accuracy achieved.

Place [x] for what applies.


## UML Diagram

Add your UML class diagram here.

Replace the following image with your diagram. You can draw it by hand and take a photo.
![UML Class Diagram](UML_class.png)

## Answers

1. How do you train the model and how do you classify a new tweet? Give a short description of the main steps.

    > To train the model, I first take each Tweet and tokenize it by words. Then, I clean each string of non-alphabetic characters and put it through a stemmer to get the root form of the word. After that, I make sure that the word is not considered a Stop Word before adding the Tweet's Line sentiment to a dictionary/map of the words encountered. Thus, if a word occurs more often in positive Tweets, then it will have a high, positive sentiment (or highly negative for negative words, and close to zero for words that occur often in both types). I then check if the word qualifies as a Hot Stop Word (words that were not included in the original Stop Word list, but should qualify as a Stop Word based on the number of occurances and variation in sentiment). Once I finish the map for all the words in all the training tweets, I take a power curve of all the evaluated word sentiments. This helps to dampen the impact of Highly Negative words during testing.
    > For classifying a new Tweet in the testing stage, I tokenize each Tweet by words, clean each word and take its stem, then add the evaluated sentiment of that word (from the map generatetd in the training set). Then, I add a factor called The Good of Humanity to each word weight. This value assumes an inherent good within humans, so if a word is on the border between being Positive or Negative, then it pushes it over the edge and considers it positive. I then take the sum of all of the word weights in the tweet. If the sum of the words is positive, then the Tweet is considered positive and vice versa. 

2. How long did your code take for training and what is the time complexity of your training implementation (Big-Oh notation)? Remember that training includes reading the tweets, breaking it into words, counting, ... Explain why you get this complexity (e.g., what does `N` stand for and how do your data structures/algorithms affect the complexity).

   > My code takes 1120 ms on average to train on a set of 20,000 tweets. For training, it has a Big-Oh time complexity of O(N). It has this complexity because it only looks at each word in each tweet a single time, so the code is only dependent on the total number of words, `N`. There are no nested loops, recursion, or sorting implementations, so the growth is linear.

3. How long did your code take for classification and what is the time complexity of your classification implementation (Big-Oh notation)? Explain why.

   > My code takes 780 ms on average to classify a set of 10,000 tweets. The time complexity is O(N) because it only looks over each word one time, so the code is only dependent on the total number of words in the tweets, `N`. There are no nested loops, recursion, or sorting implementations, so the growth is linear.

4. What accuracy did your algorithm achieve on the provided training and test data? 

   > My accuracy: 72.210%

   > Note: I talked to Hansler, and he said that the 72% extra credit allows for a ±2.0% variation due to natural variations in the tweets. So if the accuracy of the new testing set is 70.000% or higher, then I should be awarded the extra credit. 

   The TA will run your code on Linux and that accuracy value will be used to determine your grade.
   

5. What were the changes that you made that improved the accuracy the most?
   
   > My base accuracy (just using word weights), was 61.5%. Implementing a Stop Word list improved the total accuracy by 6.7%. Hot Stop Words also had a significant impact, with a 2.5% increase in accuracy. Finally using a power curve to dampen the training word weights improved my accuracy by another 1.5%, pushing me over the 72.000% bonus accuracy.

6. How do you know that you use proper memory management? I.e., how do you know that you do not have
   a memory leak?

   > By running `valgrind` on my code, I can ensure that there are no memory leaks. Proper use of desconstructors and copy constructors guaranteed that all memory leaks would be accounted for, and Valgrind confirmed this with the message: `All heap blocks were freed -- no leaks are possible`.

6. What was the most challenging part of the assignment?

   > The most challenging part of this assignment was building the DSString class. I have taken strings for granted all these years, and building my own string class got me really familiar with pointers and memory management. It only took me a few days to build the Sentiment Analyzer, but it took me about a week and a half to get the DSString fully functional with proper memory management.