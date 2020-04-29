// **********************************************************************
// * Name: Christelle Nieves                                            *
// * Class: COP 4530                                                    *                                             *
// * Program Name: Word/Number/Character Usage Statistics   			*
// * Date: April 22, 2020                                               *                                             *
// **********************************************************************

#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;

// Function Prototypes
void scanInput();
void addWord(string);
void addChar(char);
void addNum(string);
void displayResults();
int findAsciiValue(string);
void printTopChars(vector< pair<string, int> >);
void printTopWords(vector< pair<string, int> >);
void printTopNums(vector< pair<string, int> >);
bool sortChars(const pair<string, int> &, const pair<string, int> &);
bool sortWords(const pair<string, int> &, const pair<string, int> &);
bool sortNums(const pair<string, int> &, const pair<string, int> &);

vector<string> nums_inorder; // Holds numbers in the order they were processed
vector<string> words_inorder; // Holds words in the order they were processed
unordered_map<char, int> chars; // Holds all characters with their respective counts
unordered_map<string, int> nums; // Holds all numbers with their respective counts
unordered_map<string, int> words; // Holds all words with their respective counts
unordered_map<string, int> wordsWithIndexes; // Holds <word, index>
unordered_map<string, int> numsWithIndexes; // Holds <number, index>

int main()
{
    scanInput();
    displayResults();

    return 0;
}

// **********************************************************************
// * Function Name: scanInput()                                         *
// * Description: Reads a file from stdin and stores all data in maps   *
// * Parameter Description: Takes in no parameters.                     *
// * Date: April 22, 2020                                               *
// * Author: Christelle Nieves                                          *
// **********************************************************************
void scanInput()
{
    char c;
    string newWord, newNum;
    
    // Scan input from stdin until we reach EOF
    while ((c = getchar()) != EOF)
    {
        // If the char is a symbol/punctuation character, add its ascii value to the char map
        if (ispunct(c) || c == '\n' || c == ' ' || c == '\t')
        {
            addChar((int)c);
        }
        else if (isalpha(c))
        {
            // If we encounter a letter, consume letters until we encounter one that is not a letter, and
            // store the whole thing as a word, while also storing each individual letter as a character.
            while (isalpha(c))
            {
                // Add the character to the word string
                newWord += tolower(c);
                addChar((int)c);
                c = getchar();

                if (!isalpha(c))
                {
                    ungetc(c, stdin);
                    break;
                }  
            }
            // Add the word to the word map and reset the newWord string
            addWord(newWord);
            newWord = "";
        }
        else if (isdigit(c))
        {
            // If we encounter a number, continue to consume numbers until we encounter a nondigit character
            while(isdigit(c))
            {
                // Add the number to the number string and also add the number as a character
                newNum += tolower(c);
                addChar((int)c);
                c = getchar();

                if (!isdigit(c))
                {
                    ungetc(c, stdin);
                    break;
                }
            }
            // Add the full number to the nums map and reset the newNum string
            addNum(newNum);
            newNum = "";
        }
    }
}

// **********************************************************************
// * Function Name: addChar ()                                          *
// * Description: Adds a char to the char map & Increments the count    *
// * Parameter Description: Takes in a char. Returns nothing.           *
// * Date: April 22, 2020                                               *
// * Author: Christelle Nieves                                          *
// **********************************************************************
void addChar(char c)
{
    chars[c]++;
}

// ******************************************************************************
// * Function Name: addWord()                                                   *
// * Description: Stores a string in the string map & increments the count      *
// * Parameter Description: Takes in a string. Returns nothing.                 *
// * Date: April 22, 2020                                                       *
// * Author: Christelle Nieves                                                  *
// ******************************************************************************
void addWord(string s)
{
    // If we have not encountered the word yet, add it to the vector of words
    if (words.find(s) == words.end())
    {
        wordsWithIndexes[s] = words_inorder.size(); // Add the index as the value.. <number, index>
        words_inorder.push_back(s); // This is solely for the purpose of preserving the order in which numbers are encountered
    }

    // Increment the count for the word
    words[s]++;
}

// **************************************************************************************
// * Function Name: addNum  ()                                                          *
// * Description: Adds a string holding a number to the num map & increments the count  *
// * Parameter Description: Takes in a string holding the number. Returns nothing       *
// * Date: April 22, 2020                                                               *
// * Author: Christelle Nieves                                                          *
// **************************************************************************************
void addNum(string n)
{
    // If the num has not been encountered yet, add it to the nums vector
    if (nums.find(n) == nums.end())
    {
        numsWithIndexes[n] = nums_inorder.size(); // Add the index as the value.. <number, index>
        nums_inorder.push_back(n); // This is solely for the purpose of preserving the order in which numbers are encountered
    }

    // Increment the count for the num
    nums[n]++;
}

// **********************************************************************************************
// * Function Name: sortChars()                                                                 *
// * Description: Sorts in descending order by count. Breaks ties by comparing ascii values.    *
// * Parameter Description: Takes in two pairs <string, int>                                    *
// * Date: April 22, 2020                                                                       *
// * Author: Christelle Nieves                                                                  *
// **********************************************************************************************
bool sortChars(const pair<string, int> &a, const pair<string, int> &b)
{
    // If the counts are the same, we must sort by ASCII value
    if (a.second == b.second)
    {
        return (findAsciiValue(a.first) < findAsciiValue(b.first));
    }
    
    return (a.second > b.second);
}

// ******************************************************************************************
// * Function Name: sortWords()                                                             *
// * Description: Sorts in descending order by count. Breaks ties by comparing which word   *
// *    was encountered first when scanning the file                                        *
// * Parameter Description: Takes in two pairs <string, int>                                *
// * Date: April 22, 2020                                                                   *
// * Author: Christelle Nieves                                                              *
// ******************************************************************************************
bool sortWords(const pair<string, int> &a, const pair<string, int> &b)
{
    // If the counts are the same, we must sort by the order the words were encountered
    if (a.second == b.second)
    {
        return wordsWithIndexes[a.first] < wordsWithIndexes[b.first]; 
    }

    return (a.second > b.second);
}

// **************************************************************************************
// * Function Name: sortNums()                                                          *
// * Description: Sorts in descending order by count. Breaks ties by comparing which    *
// *    value was encountered first when scanning the input file.                       *
// * Parameter Description: Takes in two pairs <string, int>                            *
// * Date: April 22, 2020                                                               *
// * Author: Christelle Nieves                                                          *
// **************************************************************************************
bool sortNums(const pair<string, int> &a, const pair<string, int> &b)
{
    // If the counts are the same, we must sort by the order in which the nums were encountered
    if (a.second == b.second)
    {
        return numsWithIndexes[a.first] < numsWithIndexes[b.first]; 
    }

    return (a.second > b.second);
}

// **************************************************************************************************
// * Function Name: printTopChars()                                                                 *
// * Description: Prints the top 10 most used chars from the input file                             *
// * Parameter Description: Takes in a vector of pairs containing the characters and their counts.  *
// * Date: April 22, 2020                                                                           *
// * Author: Christelle Nieves                                                                      *
// **************************************************************************************************
void printTopChars(vector< pair<string, int> > allchars)
{
    cout << endl;
    cout << "There are " << allchars.size() << " unique characters. ";
    cout << "The top 10 characters are: " << endl;
    cout << "Index" << setw(10) << "Item" << setw(10) << "Number" << endl;

    // Loop through the allchars vector and print the contents and their corresponding values
    for (int i = 0; i < allchars.size(); i++)
    {
        if (allchars.at(i).first == "\n")
        {
            cout << "[ " << i << " ]" << setw(10) << "\\n" << setw(10) << allchars.at(i).second << endl;
        }
        else if (allchars.at(i).first == "\t")
        {
            cout << "[ " << i << " ]" << setw(10) << "\\t" << setw(10) << allchars.at(i).second << endl;
        }
        else if (allchars.at(i).first == " ")
        {
            cout << "[ " << i << " ]" << setw(10) << "Space" << setw(10) << allchars.at(i).second << endl;
        }
        else
        {
            cout << "[ " << i << " ]" << setw(10) << allchars.at(i).first << setw(10) << allchars.at(i).second << endl;
        }

        // Only print the first 10 items
        if (i == 9) break;
    }
}

// **************************************************************************************************
// * Function Name: printTopWords()                                                                 *
// * Description: Prints the top 10 most used words from the input file                             *
// * Parameter Description: Takes in a vector of pairs containing the words and their counts.       *
// * Date: April 22, 2020                                                                           *
// * Author: Christelle Nieves                                                                      *
// **************************************************************************************************
void printTopWords(vector< pair<string, int> > allwords)
{
    cout << endl;
    cout << "There are " << allwords.size() << " unique words.";
    cout << "The top 10 words are: " << endl;
    cout << "Index" << setw(12) << "Item" << setw(12) << "Number" << endl;

    // Loop through the allwords vector and print all the contents
    for (int i = 0; i < allwords.size(); i++)
    {
        cout << "[ " << i << " ]" << setw(10) << allwords.at(i).first << setw(10) << allwords.at(i).second << endl;

        // Only print the first 10 items
        if (i == 9) break;
    }
}

// **************************************************************************************************
// * Function Name: printTopNums    ()                                                              *
// * Description: Prints the top 10 most used numbers from the input file                           *
// * Parameter Description: Takes in a vector of pairs containing the numbers and their counts.     *
// * Date: April 22, 2020                                                                           *
// * Author: Christelle Nieves                                                                      *
// **************************************************************************************************
void printTopNums(vector< pair<string, int> > allnums)
{
    cout << endl;
    cout << "There are " << allnums.size() << " unique numbers.";
    cout << "The top 10 numbers are: " << endl;
    cout << "Index" << setw(12) << "Item" << setw(12) << "Number" << endl;

    // Loop through the allnums vector and print all the contents
    for (int i = 0; i < allnums.size(); i++)
    {
        cout << "[ " << i << " ]" << setw(10) << allnums.at(i).first << setw(10) << allnums.at(i).second << endl;

        // Only print the first 10 items
        if (i == 9) break;
    }
}

// **************************************************************************************************
// * Function Name: findAsciiValue()                                                                *
// * Description: Returns the ascii value of a character                                            *
// * Parameter Description: Takes in a string representing a character                              *
// * Date: April 22, 2020                                                                           *
// * Author: Christelle Nieves                                                                      *
// **************************************************************************************************
int findAsciiValue(string s)
{
    // Since the chars map holds strings, return the corresponding ascii value of the char the string represents
    if (s == "space")
    {
        return 32;
    }
    else if (s == "\n")
    {
        return 10;
    }
    else if (s == "\t")
    {
        return 9;
    }
    else
    {
        return (int)s[0];
    }
}

// **************************************************************************************************
// * Function Name: displayResults()                                                                *
// * Description: Displays the top 10 chars,nums, and words found in the input file                 *
// * Parameter Description: Has no parameters                                                       *
// * Date: April 22, 2020                                                                           *
// * Author: Christelle Nieves                                                                      *
// **************************************************************************************************
void displayResults()
{
    vector< pair<string, int> > allchars;
    vector< pair<string, int> > allwords;
    vector< pair<string, int> > allnums;

    // Copy over the chars to the vector for sorting purposes (maps cannot be sorted)
    for (auto x : chars)
    {
        string s(1, x.first);
        pair<string, int> newPair (s, x.second);
        allchars.push_back(newPair);
    }

    // Copy over the words
    for (auto x : words)
    {
        pair<string, int> newPair (x.first, x.second);
        allwords.push_back(newPair);
    }

    // Copy over the nums
    for (auto x : nums)
    {
        pair<string, int> newPair(x.first, x.second);
        allnums.push_back(newPair);
    }

    // Sort the data in descending order by count
    sort(allchars.begin(), allchars.end(), sortChars);
    sort(allwords.begin(), allwords.end(), sortWords);
    sort(allnums.begin(), allnums.end(), sortNums);

    // Print the output
    printTopWords(allwords);
    printTopNums(allnums);
    printTopChars(allchars);
}