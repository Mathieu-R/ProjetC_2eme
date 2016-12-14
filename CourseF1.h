#ifndef _COURSEF1H_ 
#define _COURSEF1H_

/**
* Struct Pilote
* Les temps sont en millisecondes
* 1 struct pour toute les infos d'un pilote
* S => Secteur
* best => Meilleur temps
*/

/* Les structures ne doivent être déclarées qu'une seule fois ! */

typedef struct Pilote {
    //le numéro du pilote
    int pilote_id;
    //Les temps des différents secteurs
    int s1;
    int bestS1;
    int s2;
    int bestS2;
    int s3;
    int bestS3;

    // Meilleur temps pour le circuit complet
    int best;
    int totalTime;

    int isPit;
    int hasGivenUp;
    int hasGivenUpDuringRace;
    int numberOfPits;

} Pilote;

sem_t semaph;

float ranf();
float randGaussien(float m, float s);
int genTime(const int min, const int max);
int genRaceEvents(const int max);
int compareBest(const void *p1, const void *p2);
int compareTot(const void *p1, const void *p2);
int forking(Pilote *p, char* name); 
int run (Pilote *p, char* name);
void fillTab(struct Pilote tabToFill[], struct Pilote tabFiller[], const int start, const int stop);

#endif