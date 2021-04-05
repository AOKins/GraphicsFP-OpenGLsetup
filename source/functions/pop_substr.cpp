#include <string>

// Method for getting a substring along with removing it from the original string, goes from start of the original string to matching endPattern
// Input: myString - original string
//        endPattern - string that is to be found as the end of the substr needing to be removed and returned
// Output: myString has the beginning portion leading up to endPattern removed with that portion returned
std::string pop_substr(std::string &myString, std::string endPattern) {
    std::string result = "";
    // Find location where the identified pattern is
    unsigned int loc = myString.find_first_of(endPattern);
    // Get the substring of the original string that leads up to the end pattern
    result = myString.substr(0,myString.length() - loc);
    // Clip the original string
    myString = myString.substr(loc+1);
    // Return the found substring
    return result;
}
