/* 
 * File:   Util.h
 * Author: Kapil Thakkar
 
 */

#ifndef UTIL_H
#define	UTIL_H

#include <random>

/**
 * Utility Function to split string message, using give delimiter. The result is stored in result[] array.
 * 
 * @param message string to be splitted
 * @param delimter delimiter used to split string
 * @param result result will be stored in this array
 * @param expcted length of the result
 */
void splitString(string message, string delimiter, string result[], int n) {
    int i = 0, pos = 0, length = 0, temp;
    temp = message.find ( delimiter.c_str ( ), pos );
    while ( temp != -1 )
    {
        length = temp - pos;
        result[i] = message.substr ( pos, length );
        pos = temp + delimiter.size ( );
        temp = message.find ( delimiter.c_str ( ), pos );
        i++;
    }
    result[i] = message.substr ( pos );
    i++;
    if ( i != n )
    {
        cout << "The similarity matrix does not have the correct format.";
        exit ( 0 );
    }
}



int * get2RandomPapers(int parallelTracks,int sessionsInTrack,int papersInSession) {
    int papers[6];

    default_random_engine generator;
    uniform_int_distribution<int> distributionTrack(0,parallelTracks - 1);
    uniform_int_distribution<int> distributionSession(0,sessionsInTrack - 1);
    uniform_int_distribution<int> distributionPaper(0,papersInSession - 1);

    do {

    papers[0] = distributionTrack(generator);
    papers[1] = distributionSession(generator);
    papers[2] = distributionPaper(generator);
    papers[3] = distributionTrack(generator);
    papers[4] = distributionSession(generator);
    papers[5] = distributionPaper(generator);
    } while (papers[1] != papers[4]);

    return papers;
}

#endif	/* UTIL_H */

