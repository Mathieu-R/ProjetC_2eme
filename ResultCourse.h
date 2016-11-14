#ifndef _RESULTCOURSEH_
#define _RESULTCOURSEH_

// typedef struct Pilote {
//     //le numéro du pilote
//     int pilote_id;

//     //Les temps des différents secteurs
//     int s1;
//     int bestS1;
//     int s2;
//     int bestS2;
//     int s3;
//     int bestS3;

//     // Meilleur temps pour le circuit complet
//     int best;

//     int isPit;
//     int hasGivenUp;
//     int hasGivenUpDuringRace;
//     int numberOfPits;

// } Pilote;

// int compare(const void *p1, const void *p2);
void showResults(struct Pilote tab[], int nbElems);

#endif