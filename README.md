# QuickSort-Time-Analysis
QuickSort implementation and time tests

# Usage: 
./executable \<numberOfItemsToSort> \<[randomized | deterministic]> \<inputPath> \<outputPath>

eg.: ./main.exe 1000 randomized tweets_unsorted.csv tweets_sorted_1000.csv

# What it does?
Sorts the first \<numberOfItemsToSort> tweets in the inputFile according to
- country (case insensitive alphabetical order), if countries match:
- artist name (case insensitive alphabetical order), if artist names also match:
- tweet_unixtime (ascending order)

# Parameters
- numberOfItemsToSort : Specify how many tweets you want to sort
- randomized : Uses Randomized pivoting for the QuickSort algorithm
- deterministic : Uses deterministic pivoting for the QuickSort algorithm
- inputPath : A '.csv' file in appropriate structure to read and sort from
- outputPath : A file to output the sort results in the same structure as input '.csv'
