/* 
 * File:   main.cpp
 * Author: Kapil Thakkar
 *
 */

#include <cstdlib>

#include "SessionOrganizer.h"

using namespace std;

/*
 * 
 */
int main(int argc, char **argv)
{
    // clock_t begin = clock();
    // Parse the input.
    if (argc < 3)
    {
        cout << "Missing arguments\n";
        cout << "Correct format : \n";
        cout << "./main <input_filename> <output_filename>";
        exit(0);
    }
    string inputfilename(argv[1]);

    // Initialize the conference organizer.
    SessionOrganizer *organizer = new SessionOrganizer(inputfilename);

    // Organize the papers into tracks based on similarity.
    // organizer->organizePapers();
    organizer->simulatedAnnealing();
    organizer->printSessionOrganiser(argv[2]);

    // Score the organization against the gold standard.

    double score = organizer->scoreOrganization(organizer->conference);
    cout << "score:" << score << endl;

    // clock_t end = clock();
    // cout<<"Seconds Taken = "<<double(end-begin) / CLOCKS_PER_SEC;

    return 0;
}
