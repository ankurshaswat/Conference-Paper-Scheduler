/* 
 * File:   Util.h
 * Author: Kapil Thakkar
 
 */

#ifndef UTIL_H
#define	UTIL_H

#include <random>
#include "Conference.h"
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
    int* papers = new int[6];

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
    } while (!(papers[1] == papers[4] && papers[0] == papers[3]));

    return papers;

}

double  swappedScore(int* paper1,int* paper2,double oldScore,double** distanceMatrix,double c,Conference* conference){

    double newScore = oldScore;

    int p1T = paper1[0] ;
    int p1P = paper1[1]  ;
    int p1K = paper1[2]   ;   
    int p2T = paper2[0];
    int p2P = paper2[1] ; 
    int p2K = paper2[2]  ;

    int *p1Row = conference->getAllParallel(p1T,p1P,p1K) ; 
    int *p2Row = conference->getAllParallel(p2T,p2P,p2K);
    int *p1Session = conference->getPapersInCurrentSession(p1T,p1P,p1K) ;
    int *p2Session = conference->getPapersInCurrentSession(p2T,p2P,p2K) ;

    int paper1InDM = conference->getPaper(p1T,p1P,p1K) ; 
    int paper2InDM = conference->getPaper(p2T,p2P,p2K) ; 

    int tracksNo = conference->getParallelTracks();
    int sessionsInTrack = conference->getSessionsInTrack() ;
    int papersInSession = conference->getPapersInSession() ; 

    for(int i = 0 ; i < tracksNo*(papersInSession-1);i+=1) {
        // cout<<p1Row[i]<<' '<<paper1InDM<<endl;
        // cout<<distanceMatrix[p1Row[i]][paper1InDM]<<endl;
        newScore -= c*distanceMatrix[p1Row[i]][paper1InDM] ;
        newScore -= c*distanceMatrix[p2Row[i]][paper2InDM] ;
        newScore += c*distanceMatrix[p1Row[i]][paper2InDM] ;
        newScore += c*distanceMatrix[p2Row[i]][paper1InDM] ;
    }

    for(int i = 0 ; i < papersInSession -1 ; i+=1) {
        // cout<<p1Session[i]<<' '<<paper1InDM<<endl;
        newScore += distanceMatrix[p1Session[i]][paper1InDM] ;
        newScore += distanceMatrix[p2Session[i]][paper2InDM] ;
        newScore -= distanceMatrix[p1Session[i]][paper2InDM] ;
        newScore -= distanceMatrix[p2Session[i]][paper1InDM] ;                
    }

    return newScore ; 
} 

#endif	/* UTIL_H */

