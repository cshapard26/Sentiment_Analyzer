#include "DSString.h"

/* 
 * Implement the functions defined in DSString.h. You may add more functions as needed
 * for the project. 
 *
 * Note that c-strings don's store an explicit length but use `\0` as the terminator symbol
 * but your class should store its length in a member variable. 
 * DO NOT USE C-STRING FUNCTIONS <string.h> or <cstring>.
 */  
DSString::DSString() {
    data = new char[1];
    data[0] = '\0';
    len = 0;
}

DSString::DSString(const char* theString) {
    size_t stringLen = 0;
    for (size_t i = 0; i <= 281; i++){
        if (theString[i] == '\0'){
            break;
        } 
        stringLen++;
    }
    data = new char[stringLen + 1];
    for (size_t i = 0; i < stringLen + 1; i++){
        data[i] = theString[i];
    }
    len = stringLen;
}
DSString::DSString(const std::string& theString) {
    size_t stringLen = 0;
    for (size_t i = 0; i <= 281; i++){
        if (theString[i] == '\0'){
            break;
        } 
        stringLen++;
    }
    data = new char[stringLen + 1];
    for (size_t i = 0; i < stringLen + 1; i++){
        data[i] = theString[i];
    }
    len = stringLen;
}

DSString::DSString(const DSString &theString) { 
    data = new char[theString.length() + 1];
    for (size_t i = 0; i < theString.length() + 1; i++){
        data[i] = theString[i];
    }
    len = theString.length();
}

DSString::~DSString() { 
    delete[] data;
}

DSString& DSString::operator=(const DSString& theString) { 
    delete[] data;
    data = new char[theString.length() + 1];
    for (size_t i = 0; i < theString.length() + 1; i++){
        data[i] = theString[i];
    }
    len = theString.length();
    return *this;
}

size_t DSString::length() const { 
    return len;
}

char& DSString::operator[](size_t theIndex) const{ 
    try {
        if (theIndex > len) {
            throw std::runtime_error("Error: Index out of range. ");
        }
        return *(data + theIndex);
    } catch (std::runtime_error& err) {
        std::cerr << err.what() << "Object: " << data << '\n';
        return *(data);
    }
}
 
DSString DSString::operator+(const DSString &theString) const { 
    char* fullStr = new char[len + theString.length() + 1];
    for (size_t i = 0; i < this->length() + 1; i++){
        fullStr[i] = data[i];
    }
    for (size_t i = 0; i <= theString.length(); i++){
        fullStr[i] = theString.data[i];
    }
    return DSString(fullStr);
}

DSString DSString::operator+(const char theChar) const { 
    char* fullStr = new char[len + 2];
    for (size_t i = 0; i < len; i++){
        fullStr[i] = data[i];
    }
    fullStr[len] = theChar;
    fullStr[len + 1] = '\0';
    DSString returnDS = DSString(fullStr);
    delete[] fullStr;
    return returnDS;
}

bool DSString::operator==(const DSString &theString) const { 
    if (len != theString.length()) {
        return false;
    }
    else {
        for (size_t i = 0; i < len; i++) {
            if (theString[i] != data[i]) {
                return false;
            }
        }
        return true;
    }
}

bool DSString::operator<(const DSString &theString) const { 
    for(size_t i = 0; i < this->len; i++) {
        if (int(data[i]) < int(theString[i])) {
            return true;
        } else if((int(data[i]) > int(theString[i]))) {
            return false;
        }
    }
    return false;
}

DSString DSString::substring(size_t start, size_t numChars) const { 
    DSString substr;
    if (start + numChars > len) {
        std::cerr << "Substring outside scope of string. Object: " << *this << std::endl;
        return substr;
    }
    for (size_t i = start; i < (numChars + start); i++) {
        substr = substr + data[i];
    }
    return substr;
}

DSString DSString::toLower() const { 
    for (size_t i = 0; i < len; i++) {
        if (data[i] >= 65 && data[i] <= 90) {
            data[i] = data[i] + 32;
        }
    }
    return *this;
}

char* DSString::c_str() const { 
    return data;
}

std::string DSString::string() const { 
    std::string theString(data);
    return theString;
}

std::ostream &operator<<(std::ostream &os, const DSString &theString) { 
    for (size_t i = 0; i < theString.length(); i++){
        os << theString[i];
    }
    os.flush();
    return os;
}

std::vector<DSString> DSString::tokenize(const char splitChar) const{ 
    DSString currentWord;
    std::vector<DSString> tokenizedWords;
    bool inQuotations = false;
    for (size_t i = 0; i < len; i++) {
        if (data[i] == '\"') {
            inQuotations = !inQuotations;
            continue;
        }
        if (!inQuotations) {
            if (data[i] == splitChar) {
                if (!currentWord[0] == '\0') {
                    tokenizedWords.push_back(currentWord);
                }   
                currentWord = "";
            } else {
                  currentWord = currentWord + data[i];
            } 
        } else {
            currentWord = currentWord + data[i];
       }
    }
    tokenizedWords.push_back(currentWord); 
    return tokenizedWords;
}

bool DSFind(std::vector<DSString>& theVector, DSString theString) { 
    for (size_t i = 0; i < theVector.size(); i++) {
        if (theVector[i] == theString) {
            return true;
        }
    }
    return false;
}

DSString DSString::cleanString() const { 
    DSString cleanedString;
    for (size_t i = 0; i < len; i++){
        if ((data[i] >= 65 && data[i] <= 90) || (data[i] >= 97 && data[i] <= 122)) {
            cleanedString = cleanedString + data[i];
        }
    }
    return cleanedString.toLower();
}

int DSString::DSToInt() const { 
    if ((data[0] >= 48 && data[0] <= 57)) {
        return (data[0] - 48);
    }
    return 0;
}