#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>

#include "fonctions.h"

#define MAX_PILOTES 22
#define ID_PROJET 'P'
#define MAX_TOURS 44

int genTime(const int min, const int max) {
    srand (time(NULL));
    return ((rand() % (max-min + 1)) + min); // Génère un nombre entre min et max
}

int genRaceEvents() { // Décide des faits de courses
	//srand(time(NULL));
	return rand() % 2; // On génère le nombre, soit 0, soit 1;
}

int compare(const void *p1, const void *p2) { // Méthode de comparation pour les temps
    const struct car *elem1 = p1;
    const struct car *elem2 = p2;

    if (elem1->s1 < elem2->s1) return -1;
    if (elem1->s1 > elem2->s1) return 1;
    return 0;
}

int makePractice(struct Pilote pilote) {

	printf("Essais Libres: \n");
	printf("===========================\n\n");

	for (int i = 0; i < MAX_TOURS; i++) { // Pour chaque tour

  		if (!pilote.practice.isOut) { // Si le pilote n'a pas abandonné
  			pilote.practice.isOut = genRaceEvents();

  			if (pilote.practice.isOut) return; // Si le pilote a abandonné, on s'arrête
  		}

        else if (pilote.practice.numberOfPits < 2) { // Max 2 arrêts
        	pilote.practice.isPit = genRaceEvents();
        	if (pilote.practice.isPit) continue; // Si le pilote est au stand, on passe au tour suivant
        }

        else { // Sinon on peut faire un tour du circuit
        	int S1 = genTime(30 * 3600, 35 * 3600);
        	int S2 = genTime(50 * 3600, 55 * 3600);
        	int S3 = genTime(29 * 3600, 34 * 3600);

        	int lap = S1 + S2 + S3;

        	if (pilote.practice.s1.S1 > S1) pilote.practice.s1.bestS1 = S1; // Si c'est son meilleur S1, on modifie le meilleur s1
        	if (pilote.practice.s2.S2 > S2) pilote.practice.s2.bestS2 = S2; // Si c'est son meilleur S2, on modifie le meilleur s2
        	if (pilote.practice.s3.S3 > S3) pilote.practice.s3.bestS3 = S3; // Si c'est son meilleur S3, on modifie le meilleur s3

        	pilote.practice.s1.S1 = S1; // On notifie le temps du S1
        	pilote.practice.s2.S2 = S2; // On notifie le temps du S2
        	pilote.practice.s3.S3 = S3; // etc...

        	if (pilote.practice.best < lap) pilote.practice.best = lap; // Si c'est son meilleur temps au tour, on le notifie

        	// Affichage
        }


	}

}

int makeQualifs(struct Pilote Pilote) {

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

int makeRace(struct Pilote Pilote) {


	printf("Course: \n");
	printf("===========================\n\n");

}

int main(int argc, char const *argv[]) {

    //typedef enum { false, true } bool; // Structure qui simule un boolean

    int pilotes_numbers[MAX_PILOTES]  = {44, 6, 5, 7, 3, 33, 19, 77, 11, 27, 26, 55, 14, 22, 9, 12, 20, 30, 8, 21, 31, 94}; // Tableau contenant les numéro des pilotes
    struct Pilote pilotes[MAX_PILOTES];

    /**
     * 1 processus par pilote !!
     * Fork
     */

    typedef struct S1 {
        int S1; // Temps S1
        int bestS1; // Meilleur S1
    } s1;

    typedef struct S2 {
        int S2; // Temps S2
        int bestS2; // Meilleur S2
    } s2;

    typedef struct S3 {
        int S3; // Temps S3
        int bestS3; // Meilleur S3
    } s3;

	typedef struct Practice {

        struct S1 s1;
        struct S2 s2;
        struct S3 s3;

        int best; // Meilleur temps au tour
        int isPit; // Si le pilote est aux stands
        int isOut; // Si le pilote a abandonné
        int numberOfPits: 0; // Combien de fois le pilote s'est arrêté au stands
    } practice;

    typedef struct Qualifs {

        struct S1 s1;
        struct S2 s2;
        struct S3 s3;

        int best; // Meilleur temps au tour
        int isPit;
        int isOut;
        int numberOfPits: 0; // Combien de fois le pilote s'est arrêté au stands
    } qualifs;


    typedef struct Race {

        struct S1 s1;
        struct S2 s2;
        struct S3 s3;

        int best; // Meilleur temps au tour
        int isPit;
        int isOut;
        int numberOfPits: 0; // Combien de fois le pilote s'est arrêté au stands
    } race;

    /**
     * Struct Pilote
     * Les temps sont en millisecondes
     * 1 struct par séance
     * S => Secteur
     * best => Meilleur temps
     */

    typedef struct Pilote {

        int pilote_id;

        struct Practice practice; // Struct pour les Essais
        struct Qualifs qualifs; // Struct pour les Qualifs
        struct Race race; // Struct pour la Course


    } pilote;

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

    for (int i=0; i < MAX_PILOTES - 1; i++) {

        if (fork() == 0) {

            //pilotes[i] = Pilote pilote; // Instance de la struct
            pilotes[i].pilote_id = pilotes_numbers[i]; // Initialise le numéro du pilote

            makePractice(pilotes[i]);
            makeQualifs(pilotes[i]);
            makeRace(pilotes[i]);


        }

    }

	return 0;
}
