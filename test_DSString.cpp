#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include "DSString.h"
#include "stemmer/Porter2Stemmer.h"

/* TODO: This file is a start to test your DSString class. Add more tests and add code to check if the results are correct. */

int main()
{      
    DSString myString = "Hello, World!";
    std::cout << myString << "\n";

    // this uses the one argument constructor in DSString and then the inherited operator=
    myString = "Good bye!";
    std::cout << myString << "\n";


    std::cout << "substr: " << myString.substring(5, 3) << "\n";


    // test some operators (=, ==, [])
    DSString a = "test";
    DSString b;
    b = a;
    std::cout << b << "\n";

    std::cout << std::boolalpha;
    std::cout << (a == b) << "\n";

    b[0] = 'T';
    std::cout << "a is now: " << a << "\n";
    std::cout << (a == b) << "\n";

    // use initialization list
    std::vector<DSString> strings = {
        DSString("bb"),
        DSString("aaa"),
        DSString("ddd"),
        DSString("ee"),
        DSString("ccc")};

    // find strings
    for (const auto &s : strings)
        std::cout
            << s << "\n";

    // find implements linear search
    std::cout << "found ddd: " << (std::find(strings.begin(), strings.end(), DSString("ddd")) != strings.end()) << "\n";
    std::cout << "found z: " << (std::find(strings.begin(), strings.end(), DSString("z")) != strings.end()) << "\n";

    // sorting using the STL (note thus needs operator= and operator<)
    std::sort(strings.begin(), strings.end());

    for (const auto &s : strings)
        std::cout
            << s << "\n";

    // the data structure is sorted. Now we can do more efficient search using STL binary search
    std::cout << "found ddd: " << binary_search(strings.begin(), strings.end(), DSString("ddd")) << "\n";
    std::cout << "found z: " << binary_search(strings.begin(), strings.end(), DSString("z")) << "\n";

    // convert characters to lowercase
    DSString c = "HeLlO tHeRe!@,bB";
    std::cout << "Before: " << c << "\n";
    c.toLower();
    std::cout << "After: " << c << "\n";


    // tokenize by words
    DSString d = "Here, we have a simple sentence.";
    DSString e = "And,here,is,one,separated,by,commas.";
    DSString g = "One.more.with.\"periods.and.quotations\"";
    std::vector<DSString> ff = d.tokenize(' ');
    for (size_t i = 0; i < ff.size(); i++) {
        std::cout << ff[i] << "\n";
    }
    ff = e.tokenize(',');
    for (size_t i = 0; i < ff.size(); i++) {
        std::cout << i << ": " << ff[i] << "\n";
    }
    ff = g.tokenize('.');
    for (size_t i = 0; i < ff.size(); i++) {
        std::cout << ff[i] << "\n";
    }

    // test the DSFind helper function
    std::vector<DSString> h = {"One", "Fish", "Two", "Fish"};

    for (size_t i = 0; i < h.size(); i++) {
        std::cout << h[i] << "\n";
    }
    DSString i = "Fsh";
    std::cout << DSFind(h, i) << "\n";
    
    //test string cleaner
    DSString j = "&&d0,shJJl*s";
    std::cout << "Before: " << j << '\n';
    std::cout << "After: " << j.cleanString() << '\n';

    //stem the string
    DSString k = "smiling";
    std::string l = k.string();

    std::cout << "Before Stemming: " << l << '\n';
    Porter2Stemmer::trim(l);
    Porter2Stemmer::stem(l);
    std::cout << "After Stemming: " << l << '\n';

    return 0;

}
