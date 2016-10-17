#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>

//#include "fonctions.h"

#define MAX_PILOTES 22
#define ID_PROJET 'P'
#define MAX_TOURS 44

/**
    * Struct Pilote
    * Les temps sont en millisecondes
    * 1 struct par séance
    * S => Secteur
    * best => Meilleur temps
    */

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

    int isPit;
    int hasGivenUp;
    int numberOfPits;

} Pilote;

int genTime(const int min, const int max) {
    srand (time(NULL));
    return ((rand() % (max-min + 1)) + min); // Génère un nombre entre min et max
}

int genRaceEvents() { // Décide des faits de courses
	//srand(time(NULL));
	return rand() % 2; // On génère le nombre, soit 0, soit 1;
}

int compare(const void *p1, const void *p2) { // Méthode de comparation pour les temps
    const struct Pilote *elem1 = p1;
    const struct Pilote *elem2 = p2;

    if (elem1->best < elem2->best) return -1;
    if (elem1->best > elem2->best) return 1;
    return 0;
}

int makePractice(Pilote *p) {

	printf("Essais Libres: \n");
	printf("===========================\n\n");

	for (int i = 0; i < MAX_TOURS; i++) { // Pour chaque tour

  		if (!p->hasGivenUp) { // Si le pilote n'a pas abandonné
  			p->hasGivenUp = genRaceEvents();
            
  			if (p->hasGivenUp) { // Si le pilote a abandonné, on s'arrête (on sort de la boucle)
                printf("Le pilote a abandonné au tour %d", i+1);
                return 0;
            }
  		}

        else if (p->numberOfPits < 2) { // Max 2 arrêts
        	p->isPit = genRaceEvents();
            p->numberOfPits++;
        }

        else { // Sinon on peut faire un tour du circuit
        	int S1 = genTime(30 * 3600, 35 * 3600);
        	int S2 = genTime(50 * 3600, 55 * 3600);
        	int S3 = genTime(29 * 3600, 34 * 3600);

        	int lap = S1 + S2 + S3;

        	if (p->bestS1 > S1) p->bestS1 = S1; // Si c'est son meilleur S1, on modifie le meilleur s1
        	if (p->bestS2 > S2) p->bestS2 = S2; // Si c'est son meilleur S2, on modifie le meilleur s2
        	if (p->bestS3 > S3) p->bestS3 = S3; // Si c'est son meilleur S3, on modifie le meilleur s3

        	p->s1 = S1; // On notifie le temps du S1
        	p->s2 = S2; // On notifie le temps du S2
        	p->s3 = S3; // etc...

        	if (p->best > lap) p->best = lap; // Si c'est son meilleur temps au tour, on le notifie


	    } 

    } // Fin de la boucle
}

int makeQualifs(Pilote *p) {

	printf("Qualifs (Q1): \n");
	printf("===========================\n\n");

	// Q1

	printf("Qualifs (Q2): \n");
	printf("===========================\n\n");

	// Q2


	printf("Qualifs (Q3): \n");
	printf("===========================\n\n");

	// Q3

}

int makeRace(Pilote *p) {


	printf("Course: \n");
	printf("===========================\n\n");

}

int main(int argc, char const *argv[]) {

    //typedef enum { false, true } bool; // Structure qui simule un boolean

    int pilotes_numbers[MAX_PILOTES]  = {44, 6, 5, 7, 3, 33, 19, 77, 11, 27, 26, 55, 14, 22, 9, 12, 20, 30, 8, 21, 31, 94}; // Tableau contenant les numéro des pilotes

    /**
     * 1 processus par pilote !!
     * Fork
     */

    //Tableau de structures pilote.
     struct Pilote pilotesTab[MAX_PILOTES];

    /**
     * Mise en place de la shared memory
    */

	/*key_t key; // Clé
    key = ftok(argv[0], ID_PROJET); // argv[O] => nom du programme lancé, ID (char)
	int perm = 0644; // Permissions

	int shmid = 0; // ID de la shared memory

	shmid = shmget(key, sizeof(Pilote), 0666);

	if (shmid == -1) {
		printf("ERREUR: BAD SHARED MEMORY ALLOCATION.");
		return 0;
	}*/

    for (int i=0; i < MAX_PILOTES; i++) {

        //pilotes[i] = Pilote pilote; // Instance de la struct
        pilotesTab[i].pilote_id = pilotes_numbers[i]; // Initialise le numéro du pilote

        //pilotesTab[i] pilo
        makePractice(&pilotesTab[i]);

        // QSORT

        // Affichage

        qsort(pilotesTab, MAX_PILOTES, sizeof(Pilote), compare); 

        for (int i = 0; i<MAX_PILOTES; i++) {
            printf("%d%s%d%s%d%s%d%s%d%s\n" ,i+1,": voiture n°", pilotesTab[i].pilote_id,": ", pilotesTab[i].best,"s (", pilotesTab[i].best/60,"m", pilotesTab[i].best%60,"s)"); 
        }

     
    }

    //makeQualifs(&pilotes[i]);
    //makeRace(&pilotes[i]);

	return 0;
}

