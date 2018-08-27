/* 
 * File:   SessionOrganizer.cpp
 * Author: Kapil Thakkar
 * 
 */

#include "SessionOrganizer.h"
#include "Util.h"

SessionOrganizer::SessionOrganizer ( )
{
    parallelTracks = 0;
    papersInSession = 0;
    sessionsInTrack = 0;
    processingTimeInMinutes = 0;
    tradeoffCoefficient = 1.0;
}

SessionOrganizer::SessionOrganizer ( string filename )
{
    readInInputFile ( filename );
    conference = new Conference ( parallelTracks, sessionsInTrack, papersInSession );
    
    int paperCounter = 0;
    
    for ( int i = 0; i < sessionsInTrack; i++ )
    {
        for ( int j = 0; j < parallelTracks; j++ )
        {
            for ( int k = 0; k < papersInSession; k++ )
            {
                conference->setPaper ( j, i, k, paperCounter );
                paperCounter++;
            }
        }
    }

    conference->setScore(scoreOrganization());
}

void SessionOrganizer::organizePapers ( )
{

    double ** distanceMatrix = this->getDistanceMatrix();

    /**
     * Fast Local Search
    **/
    // double oldScore = conference->getScore();
    // for(int i =0;i < 1000;i++) {
    //     int* papers = get2RandomPapers(parallelTracks, sessionsInTrack, papersInSession);
    //     bool swapped = false;
    //     int paper1[3];
    //     paper1[0] = papers[0];
    //     paper1[1] = papers[1];
    //     paper1[2] = papers[2];
    //     for (int x=0;x<parallelTracks;x++) {
    //         if(swapped == true) {
    //             break;
    //         }
    //         for(int y=0;y<sessionsInTrack;y++) {
    //             if(swapped == true) {
    //                 break;
    //             }
    //             if (x== paper1[0] && y == paper1[1]) {
    //                 continue;
    //             }
    //             for(int z=0;z<papersInSession;z++) {
    //                 int paper2[3];
    //                 paper2[0] = x;
    //                 paper2[1] = y;
    //                 paper2[2] = z;
    //                 double newScore = swappedScore(paper1,paper2,oldScore,this->getDistanceMatrix(),tradeoffCoefficient,conference);
    //                 if(newScore > oldScore) {
    //                     conference->swap(paper1,paper2,newScore);
    //                     oldScore = newScore;
    //                     swapped = true;
    //                     break;
    //                 } 
    //             }
    //         }
    //     }
    // }

    /**
     * Best Neighbour Local Search
    **/
    double oldScore = conference->getScore();
    for(int i =0;i < 10000;i++) {
        int* papers = get2RandomPapers(parallelTracks, sessionsInTrack, papersInSession);
        int paper1[3];
        paper1[0] = papers[0];
        paper1[1] = papers[1];
        paper1[2] = papers[2];
        int bestNeightbour[3];
        int bestScore = oldScore;
        for (int x=0;x<parallelTracks;x++) {
            for(int y=0;y<sessionsInTrack;y++) {
                if (x== paper1[0] && y == paper1[1]) {
                    continue;
                }
                for(int z=0;z<papersInSession;z++) {
                    int paper2[3];
                    paper2[0] = x;
                    paper2[1] = y;
                    paper2[2] = z;
                    double newScore = swappedScore(paper1,paper2,oldScore,distanceMatrix,tradeoffCoefficient,conference);
                    if(newScore > bestScore) {
                        bestNeightbour[0] = x;
                        bestNeightbour[1] = y;
                        bestNeightbour[2] = z;
                        bestScore = newScore;
                        cout<<"New Best Score "<<bestScore<<endl;
                    }
                }
            }
        }
        conference->swap(paper1,bestNeightbour,bestScore);
        oldScore = bestScore;
    }
    
}

void SessionOrganizer::readInInputFile ( string filename )
{
    vector<string> lines;
    string line;
    ifstream myfile ( filename.c_str () );
    if ( myfile.is_open ( ) )
    {
        while ( getline ( myfile, line ) )
        {
            lines.push_back ( line );
        }
        myfile.close ( );
    }
    else
    {
        cout << "Unable to open input file";
        exit ( 0 );
    }

    if ( 6 > lines.size ( ) )
    {
        cout << "Not enough information given, check format of input file";
        exit ( 0 );
    }

    processingTimeInMinutes = atof ( lines[0].c_str () );
    papersInSession = atoi ( lines[1].c_str () );
    parallelTracks = atoi ( lines[2].c_str () );
    sessionsInTrack = atoi ( lines[3].c_str () );
    tradeoffCoefficient = atof ( lines[4].c_str () );

    int n = lines.size ( ) - 5;
    double ** tempDistanceMatrix = new double*[n];
    for ( int i = 0; i < n; ++i )
    {
        tempDistanceMatrix[i] = new double[n];
    }


    for ( int i = 0; i < n; i++ )
    {
        string tempLine = lines[ i + 5 ];
        string elements[n];
        splitString ( tempLine, " ", elements, n );

        for ( int j = 0; j < n; j++ )
        {
            tempDistanceMatrix[i][j] = atof ( elements[j].c_str () );
        }
    }
    distanceMatrix = tempDistanceMatrix;

    int numberOfPapers = n;
    int slots = parallelTracks * papersInSession*sessionsInTrack;
    if ( slots != numberOfPapers )
    {
        cout << "More papers than slots available! slots:" << slots << " num papers:" << numberOfPapers << endl;
        exit ( 0 );
    }
}

double** SessionOrganizer::getDistanceMatrix ( )
{
    return distanceMatrix;
}

void SessionOrganizer::printSessionOrganiser ( char * filename)
{
    conference->printConference ( filename);
}

double SessionOrganizer::scoreOrganization ( )
{
    // Sum of pairwise similarities per session.
    double score1 = 0.0;
    for ( int i = 0; i < conference->getParallelTracks ( ); i++ )
    {
        Track tmpTrack = conference->getTrack ( i );
        for ( int j = 0; j < tmpTrack.getNumberOfSessions ( ); j++ )
        {
            Session tmpSession = tmpTrack.getSession ( j );
            for ( int k = 0; k < tmpSession.getNumberOfPapers ( ); k++ )
            {
                int index1 = tmpSession.getPaper ( k );
                for ( int l = k + 1; l < tmpSession.getNumberOfPapers ( ); l++ )
                {
                    int index2 = tmpSession.getPaper ( l );
                    score1 += 1 - distanceMatrix[index1][index2];
                }
            }
        }
    }

    // Sum of distances for competing papers.
    double score2 = 0.0;
    for ( int i = 0; i < conference->getParallelTracks ( ); i++ )
    {
        Track tmpTrack1 = conference->getTrack ( i );
        for ( int j = 0; j < tmpTrack1.getNumberOfSessions ( ); j++ )
        {
            Session tmpSession1 = tmpTrack1.getSession ( j );
            for ( int k = 0; k < tmpSession1.getNumberOfPapers ( ); k++ )
            {
                int index1 = tmpSession1.getPaper ( k );

                // Get competing papers.
                for ( int l = i + 1; l < conference->getParallelTracks ( ); l++ )
                {
                    Track tmpTrack2 = conference->getTrack ( l );
                    Session tmpSession2 = tmpTrack2.getSession ( j );
                    for ( int m = 0; m < tmpSession2.getNumberOfPapers ( ); m++ )
                    {
                        int index2 = tmpSession2.getPaper ( m );
                        score2 += distanceMatrix[index1][index2];
                    }
                }
            }
        }
    }
    double score = score1 + tradeoffCoefficient*score2;
    return score;
}
