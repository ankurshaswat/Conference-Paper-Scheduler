/* 
 * File:   Conference.cpp
 * Author: Kapil Thakkar
 * 
 */

#include "Conference.h"

Conference::Conference ( )
{
    this->parallelTracks = 0;
    this->sessionsInTrack = 0;
    this->papersInSession = 0;
}

Conference::Conference ( int parallelTracks, int sessionsInTrack, int papersInSession )
{
    this->parallelTracks = parallelTracks;
    this->sessionsInTrack = sessionsInTrack;
    this->papersInSession = papersInSession;
    initTracks ( parallelTracks, sessionsInTrack, papersInSession );
}

void Conference::initTracks ( int parallelTracks, int sessionsInTrack, int papersInSession )
{
    tracks = ( Track * ) malloc ( sizeof (Track ) * parallelTracks );
    for ( int i = 0; i < parallelTracks; i++ )
    {
        Track tempTrack ( sessionsInTrack );
        for ( int j = 0; j < sessionsInTrack; j++ )
        {
            Session tempSession ( papersInSession );
            tempTrack.setSession ( j, tempSession );
        }
        tracks[i] = tempTrack;
    }
}

int Conference::getParallelTracks ( )
{
    return parallelTracks;
}

int Conference::getSessionsInTrack ( )
{
    return sessionsInTrack;
}

int Conference::getPapersInSession ( )
{
    return papersInSession;
}

Track Conference::getTrack ( int index )
{
    if ( index < parallelTracks )
    {
        return tracks[index];
    }
    else
    {
        cout << "Index out of bound - Conference::getTrack" << endl;
        exit ( 0 );
    }
}

void Conference::setPaper ( int trackIndex, int sessionIndex, int paperIndex, int paperId )
{
    if ( this->parallelTracks > trackIndex )
    {
        Track curTrack = tracks[trackIndex];
        curTrack.setPaper ( sessionIndex, paperIndex, paperId );
    }
    else
    {
        cout << "Index out of bound - Conference::setPaper" << endl;
        exit ( 0 );
    }
}

int Conference::getPaper( int trackIndex, int sessionIndex, int paperIndex) {
 return tracks[trackIndex].getSession ( sessionIndex ).getPaper ( paperIndex );
}

int *Conference::getAllParallel(int trackIndex, int sessionIndex, int paperIndex){

    int papersInSession = getPapersInSession() ;
    int parallelTracks = getParallelTracks() ; 
    int *rowIndices = new int[parallelTracks*(papersInSession -1)];
    
    for(int i = 0 ; i < getParallelTracks( ); i += 1) {
        if(i != trackIndex) {
            for(int j = 0 ; j < papersInSession ; j += 1 ){
                rowIndices[i*getParallelTracks( ) + j ]= getPaper(i,sessionIndex,j) ; 
            }
        }
    }

    return rowIndices ; 
}

int *Conference::getPapersInCurrentSession(int trackIndex, int sessionIndex, int paperIndex){
    int papersInSession = getPapersInSession() ; 
    int *currSessionIndices =  new int[papersInSession - 1];
    for(int i = 0 ; i < papersInSession ; i += 1) {
        if(i !=  paperIndex){
            currSessionIndices[i] = getPaper(trackIndex,sessionIndex,i) ;  
        }
    }

    return currSessionIndices ; 
    
}


void Conference::printConference (char * filename )
{
    ofstream ofile(filename);

    for ( int i = 0; i < sessionsInTrack; i++ )
    {
        for ( int j = 0; j < parallelTracks; j++ )
        {
            for ( int k = 0; k < papersInSession; k++ )
            {
                ofile<< getPaper(j,i,k) << " ";
            }
            if ( j != parallelTracks - 1 )
            {
                ofile<<"| ";
            }
        }
        ofile<<"\n";
    }
    ofile.close();
    cout<<"Organization written to ";
    printf("%s :)\n",filename);

}

void Conference::setScore(double score) {
    this->score = score;
}

double Conference::getScore() {
    return this->score;
}

void Conference::swap(int* papers,double newScore) {
    this->setScore(newScore);
    int oldVals[2];

    oldVals[0] = getPaper(papers[0],papers[1],papers[2]);
    oldVals[1] = getPaper(papers[3],papers[4],papers[5]);

    setPaper(papers[0],papers[1],papers[2],oldVals[1]);
    setPaper(papers[3],papers[4],papers[5],oldVals[0]);
}