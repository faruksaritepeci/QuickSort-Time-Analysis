// g++ std=c++11 ,at least
#include <iostream>
#include <ctime>
#include <string>
#include <cstring>
#include <fstream>
#include <random>

using namespace std;

//Tweet struct to store data from the input file
struct tweet{
    int id = 0;
    string dateTime;
    int unixTime = 0;
    string artistName;
    string trackTitle;
    string country;
};

void FillArrayFromFile(const string&, tweet*, int); //Fills the tweet array from the input file
void FillTweetFromString(tweet&, char[]); //Fills a tweet struct element from a given appropriate string
void QuickSort(tweet*, int, int, bool); //The main quicksort function which takes a tweet array as an input
void PrintOutputFile(const string&, tweet*, int); //fills an output file from a given tweet array appropriately
bool CompareTweets(const tweet &tweet1, const tweet &tweet2); //compares 2 tweets and returns true if the first one is "bigger"
int PivotSel(bool isRandom, int min, int max); //Pivot selection function which selects randomly or deterministically
void SwapTweets(tweet* array, int member1, int member2); //swap places of 2 tweets in a tweet array
int RandomNumber(int min, int max); //generate high sensitive random number

//Global variables:
mt19937 rng(time(0)); //This global class variable is used to generate high sensitive random number in RandomNumber()
//Initialize with seed: current time. (included from <random>)

int main(int argc, char** argv) {
    if (argc < 5) {return 0;} //If there is fewer arguments than needed, terminate program without running.

    int numberOfTweets = stoi(argv[1]); //first argument is number of tweets which will be turned into an integer
    string pivotSel = argv[2]; //random or deterministic pivot selection for quicksort
    string inputPath = argv[3]; //path of input file to fill tweet array
    string outputPath = argv[4]; //path to output file after sorting

    //srand(time(0)); //is not precise as it only goes up to 32767 (RAND_MAX), while tweet numbers goes up to 1 million

    tweet* tweetArray = new tweet[numberOfTweets]; //allocate space for tweet array

    FillArrayFromFile(inputPath, tweetArray, numberOfTweets); //fill tweetArray

    clock_t startTime, finishTime; //use these 2 time variables to calculate total time spent in quicksort

    startTime = clock(); //time when quicksort starts
    // Quicksort(array, min = array index to start sorting, max = array index to sort until, isRandom = True if Random, False if deterministic)
    if (pivotSel == "randomized"){ QuickSort(tweetArray, 0, numberOfTweets - 1, true);}
    else if (pivotSel == "deterministic"){ QuickSort(tweetArray, 0, numberOfTweets - 1, false);}
    finishTime = clock(); //time when quicksort finishes

    double quickSortTime = double(finishTime - startTime) / double(CLOCKS_PER_SEC); //Calculate total time spent in QS
    cout << "Sorted in " << quickSortTime * 1000.0 << " milliseconds by using " << pivotSel << " pivot selection." << endl;

    PrintOutputFile(outputPath, tweetArray, numberOfTweets); //Save sorted array to a given output file path in correct format

    delete[] tweetArray; //delete allocated space for tweetArray
    return 0;
}

void FillArrayFromFile(const string &inputPath, tweet* array, int numberOfTweets)
{
    fstream inputFile;
    inputFile.open(inputPath, ios::in); //Open file in inputPath for input
    char temp[200]; //temporary char array to store line of text from file

    inputFile.getline(temp,200); //Skip the first line from file which is unused

    for (int i = 0; i < numberOfTweets; i++) //Fill array until numberOfTweets
    {
        inputFile.getline(temp,200); //Get the line of string from where it is left (until '\n')
        //cout << "Line - " << i << " : " << temp << endl; //DEBUG*
        FillTweetFromString(array[i], temp); //call this function to store the info to the tweet variable on array[i]
    }



    inputFile.close(); //close input file
}

void FillTweetFromString( tweet &tweetUnit, char tweetDat[]) //Get a reference to the tweet variable and fill it
{
    int counter = 0; //to store the index of char[] while reading
    string temp; //create temp string to store a certain variable of tweet struct

    while (tweetDat[counter] != '\t') //read until tab and get first variable: id
    {
        temp.append(1, tweetDat[counter]); //append to temp string until tab
        counter++; //go to next character on the char[] string
    }
    counter++; //go to the element after tab
    tweetUnit.id = stoi(temp); //assign temp as integer to id variable of this tweet
    temp.clear(); //clear temp string

    while (tweetDat[counter] != '\t') //read until 2nd tab and get second variable: date time
    {
        temp.append(1, tweetDat[counter]);
        counter++;
    }
    counter++;
    tweetUnit.dateTime = temp; //assign datetime from temp
    temp.clear();

    while (tweetDat[counter] != '\t')
    {
        temp.append(1, tweetDat[counter]);
        counter++;
    }
    counter++;
    tweetUnit.unixTime = stoi(temp); //assign unix time as integer from temp
    temp.clear();

    while (tweetDat[counter] != '\t')
    {
        temp.append(1, tweetDat[counter]);
        counter++;
    }
    counter++;
    tweetUnit.artistName = temp; //assign artist name from temp
    temp.clear();

    while (tweetDat[counter] != '\t')
    {
        temp.append(1, tweetDat[counter]);
        counter++;
    }
    counter++;
    tweetUnit.trackTitle = temp; //assign track title from temp
    temp.clear();

    while (tweetDat[counter] != '\n' && tweetDat[counter] != '\0' && tweetDat[counter] != '\t') //read till '\n' for last element
    {
        temp.append(1, tweetDat[counter]);
        counter++;
    }
    counter++;
    tweetUnit.country = temp; //assign country from temp
    temp.clear();
    //Could've made this function simpler.
}

void PrintOutputFile(const string &outputPath, tweet* array, int numberOfTweets) //Save array to an output path
{
    fstream outputFile;
    outputFile.open(outputPath, ios::out); //open file on outputPath for output.

    outputFile << "tweet_id\ttweet_datetime\ttweet_unixtime\tartist_name\ttrack_title\tcountry\n"; //add header for file

    for (int i = 0; i < numberOfTweets; i++) //Add tweets 1 by 1 from array to the file in the correct format
    {
        outputFile << array[i].id << '\t' << array[i].dateTime << '\t' << array[i].unixTime << '\t' << array[i].artistName << '\t' << array[i].trackTitle << '\t' << array[i].country << '\n';
    }

    /* DEBUG*
    cout << array[999].id << '\t' << array[999].dateTime << '\t' << array[999].unixTime << '\t' << array[999].artistName << '\t' << array[999].trackTitle << '\t' << array[999].country << '\n';
    cout << array[998].id << '\t' << array[998].dateTime << '\t' << array[998].unixTime << '\t' << array[998].artistName << '\t' << array[998].trackTitle << '\t' << array[998].country << '\n';

    SwapTweets(array, 998, 999);

    cout << array[999].id << '\t' << array[999].dateTime << '\t' << array[999].unixTime << '\t' << array[999].artistName << '\t' << array[999].trackTitle << '\t' << array[999].country << '\n';
    cout << array[998].id << '\t' << array[998].dateTime << '\t' << array[998].unixTime << '\t' << array[998].artistName << '\t' << array[998].trackTitle << '\t' << array[998].country << '\n';
     */

    //cout << array[999].id << '\n' << array[999].dateTime << '\n' << array[999].unixTime << '\n' << array[999].artistName << '\n' << array[999].trackTitle << '\n' << array[999].country << '\n';

    outputFile.close(); //close output file
}

bool CompareTweets(const tweet &tweet1, const tweet &tweet2) //returns true if 1 is larger than 2, false otherwise
{
    if (strcasecmp(tweet1.country.c_str(), tweet2.country.c_str()) > 0) //compare country strings first
    {
        return true;
    }
    else if (strcasecmp(tweet1.country.c_str(), tweet2.country.c_str()) == 0) //if countries are same
    {
        if (strcasecmp(tweet1.artistName.c_str(), tweet2.artistName.c_str()) > 0) //compare artist names
        {
            return true;
        }
        else if (strcasecmp(tweet1.artistName.c_str(), tweet2.artistName.c_str()) == 0) //if artist names are same
        {
            if (tweet1.unixTime > tweet2.unixTime) //compare unix time
            {
                return true;
            }
        }
    }
    return false; //else, if above comparisons fail, tweet1 must be smaller or equal to tweet2
}

int PivotSel(bool isRandom, int min, int max) //Picks pivot between index min and max
{
    if (isRandom) //if random pivot selection is true:
    {
        //return (rand() % ((max - min) + 1)) + min; //rand is not precise enough after 32767 (RAND_MAX)
        return RandomNumber(min, max); //calls RandomNumber() function and returns it
    }
    return max; //otherwise, pick last element as the pivot (deterministic)
}

void SwapTweets(tweet *array, int member1, int member2) //function to swap 2 tweets in tweet array
{
    if (member1 == member2){return;} //if call made for the same index don't copy and swap, just return

    tweet temp = array[member2]; //copy the tweet into a temporary tweet struct
    array[member2] = array[member1]; //assign 2 as 1
    array[member1] = temp; //assign 1 as 2 from temp

}

void QuickSort(tweet* array, int min, int max, bool isRandom) //QuickSort for tweet struct (both Random and deterministic)
{
    if (min < max) //if min and max are not the same:
    {
        int pivot = PivotSel(isRandom, min, max); //select pivot (either random or deterministic according to isRandom)
        SwapTweets(array, pivot, max); //swap pivot with last element
        pivot = max; //pivot index now refers to last element

        //This part is the codified version of logic from the in-place quicksort lecture slide
        //pointer 1 is the blue line, pointer 2 is the orange line from the slides
        int pointer1 = min; //assign both at the start of the array
        int pointer2 = min; //pointer1 will point to end of Right Side of pivot, pointer2 will point to beginning of RS.
        for (int i = min; i <= max; i++) //from beginning to the end of the given array
        {
            if (CompareTweets(array[pointer1], array[pivot])) //if pointer 1 > pivot
            {
                pointer1++; //move pointer1 (blue line) by 1 unit, so we are marking the end of the right side of pivot
            }
            else //if it is smaller than pivot or equal to pivot
            {
                SwapTweets(array, pointer2, pointer1); //swap the beginning of Right Side with the current element
                pointer1++; //shift both pointers by 1 to the right side, so the Right Side stays correct
                pointer2++; //this part will also put the pivot on the right place at the last iteration
            }
        } //pointer 1 is redundant, i could be used instead, but I decided to leave it as in the visual slide.
        pivot = pointer2 - 1; //after the loop the pivot will be the element just before the start of Right Side

        QuickSort(array, min, pivot - 1, isRandom); //using recurrence sort Left Side (till pivot) and Right Side (after pivot)
        QuickSort(array, pivot + 1, max, isRandom); //no new array has been created, we are still using the same array.
        //because we used min, max everywhere and identified them correctly recurrences work perfectly with the loop and function calls.
    }
}

int RandomNumber(int min, int max) { //Generate uniform high precision integer between min and max.
    uniform_int_distribution<int> range(min, max); //create uniform distribution class named range with range min to max
    //included from <random>

    //DEBUG*
    //int randomNumber = range(rng);
    //cout << "Random Number: " << randomNumber << " ,min: " << min << " max: " << max <<  endl;
    //return randomNumber;
    //cout << "Random Function called!" << endl;
    //cout << range(rng) << endl;
    return range(rng); //global variable 'rng' is used here to generate high precision integer in range(min,max).
}


