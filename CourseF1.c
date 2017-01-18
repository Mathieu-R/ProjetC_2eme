/* /!\ COMPILER AVEC -lm /!\ */

#include <stdio.h> /* input/output */
#include <stdlib.h> /* standart librabries */
#include <unistd.h> 
#include <sys/shm.h> /* shared memory */
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h> /* time */
#include <string.h> /* string */
#include <math.h> /* math */
#include <sys/types.h> 
#include <sys/ipc.h> /* inter processus communication */
#include <sys/sem.h> /* semaphores */
#include <wait.h> /* wait */

#include "CourseF1.h"
#include "ResultCourse.h"

#define MAX_PILOTES 22
#define MAX_TOURS 44

// Variable du sémaphore
int semid; // semaphore id

float ranf() { // PRNG pour des floats [0, 1].
	float r = rand() / (float) RAND_MAX;
	return r;
}

/*
 * Cette méthode est basée sur la transformation de Box-Muller: https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
 */
float randGaussien(float m, float s) { /* median m, écart-type s */
	float x1, x2, w, y1, result;
	static float y2;
	static int use_last = 0;

	if (use_last) /* utilise la valeur de l'appel précédent */
	{
		y1 = y2;
		use_last = 0;
	}
	else
	{
		do {
			x1 = 2.0 * ranf() - 1.0;
			x2 = 2.0 * ranf() - 1.0;
			w = x1 * x1 + x2 * x2;
		} while ( w >= 1.0 );

		w = sqrt( (-2.0 * log( w ) ) / w );
		y1 = x1 * w;
		y2 = x2 * w;
		use_last = 1;
	}
	
	result = ( m + y1 * s );
	if (result < 0) {
		return 0.0;
	}
	return result;
}


int genTime(const int min, const int max) {
    return ((rand() % (max-min + 1)) + min); // Génère un nombre entre min et max
}

int genRaceEvents(const int max) { // Décide des faits de courses
	return rand() % max; // On génère le nombre entre 0 et max - 1;
}

int compareBest(const void *p1, const void *p2) { // Méthode de comparation pour les meilleurs temps
    const struct Pilote *elem1 = p1;
    const struct Pilote *elem2 = p2;

    if (elem1->best < elem2->best) return -1;
    if (elem1->best > elem2->best) return 1;
    return 0;
}

int compareTot(const void *p1, const void *p2) { // Méthode de comparation pour les temps totaux de la course
    const struct Pilote *elem1 = p1;
    const struct Pilote *elem2 = p2;
    /* Tri majeur sur l'abandon */
    if (elem1->hasGivenUpDuringRace < elem2->hasGivenUpDuringRace) return -1;
    if (elem1->hasGivenUpDuringRace > elem2->hasGivenUpDuringRace) return 1;

    /* Tri mineur sur le temps */
    if (elem1->totalTime < elem2->totalTime) return -1;
    if (elem1->totalTime > elem2->totalTime) return 1;
    return 0;
}

/**
 * Attend qu'un nombre spécifié de processus enfant soit fini
 */
void waitChildren(const int max) {
    for (int i = 0; i < max; i++) {
        wait(NULL);
    }
}

void fillTab(struct Pilote tabToFill[], struct Pilote tabFiller[], const int start, const int stop) {
    for (int i = start; i < stop; i++) {
        tabToFill[i] = tabFiller[i];
    }
}

void asciiArt() {
 printf(" 88888888b d88      888888ba                    oo                     dP   \n");
 printf(" 88         88      88    `8b                                          88   \n");
 printf("a88aaaa     88     a88aaaa8P' 88d888b. .d8888b. dP .d8888b. .d8888b. d8888P \n");
 printf(" 88         88      88        88'  `88 88'  `88 88 88ooood8 88'  `""   88   \n");
 printf(" 88         88      88        88       88.  .88 88 88.  ... 88.  ...   88   \n");
 printf(" dP        d88P     dP        dP       `88888P' 88 `88888P' `88888P'   dP   \n");
 printf("oooooooooooooooooooooooooooooooooooooooooooooooo88~ooooooooooooooooooooooooo\n");
 printf("                                                dP                          \n");
}

int run(Pilote *p, char* name) {

    /* Instancie toutes les valeurs (exepté le pilote_id) */
    p->s1 = 3 * 60 * 3600 + 1;
    p->bestS1 =  3 * 60 * 3600 + 1;
    p->s2 =  3 * 60 * 3600 + 1;
    p->bestS2 =  3 * 60 * 3600 + 1;
    p->s3 =  3 * 60 * 3600 + 1;
    p->bestS3 =  3 * 60 * 3600 + 1;
    p->best =  3 * 60 * 3600 + 1;
    p->totalTime = 0;
    p->isPit = 0;
    p->hasGivenUp = 0;
    p->hasGivenUpDuringRace = 0;
    p->numberOfPits = 0;

    for (int i = 0; i < MAX_TOURS; i++) { // Pour chaque tour

        p->isPit = 0; // Au début du tour, il n'est pas aux stands

        if (!(p->hasGivenUp)) { // Si le pilote n'a pas abandonné

            int givingUpEvent = genRaceEvents(500); // Génère un nombre entre 0 et 14

            if (givingUpEvent == 14 && strcmp(name, "Race") == 0) { // Si le pilote a abandonné durant la course
                p->hasGivenUpDuringRace = 1;
                return 0; // Stop le pilote
            }
            
            else if (givingUpEvent == 14) { // Si le pilote a abandonné (mais pas dans la course)
                p->hasGivenUp = 1;
                return 0; // Stop le pilote
            }
        }

        if (p->numberOfPits < 2) { // Max 2 arrêts
            p->isPit = (genRaceEvents(250) == 1) ? 1 : 0;

            if (p->isPit) {
                p->numberOfPits++;
                if ((strcmp(name, "Practices") == 0) || (strcmp(name, "Qualifs") == 0)) continue; // On passe à l'itération suivante
            }
            
        }

        // On peut faire un tour du circuit
        int S1 = 0.275 * (103000 + randGaussien(5000, 2000)); // Portion de circuit * Courbe de Gausse (temps min + fun(médian, écart-type))
        int S2 = 0.459 * (103000 + randGaussien(5000, 2000)); 
        int S3 = 0.266 * (103000 + randGaussien(5000, 2000)); 

        if ((strcmp(name, "Race") == 0) && (p->isPit)) { // Si l'on est en course et que le pilote est au stand
            S1 += genTime(20 * 3600, 25 * 3600); // On rajoute entre 20 et 25sec au Secteur 1
        }

        p->s1 = S1; // On notifie le temps du S1
        p->s2 = S2; // On notifie le temps du S2
        p->s3 = S3; // etc...

        int lap = S1 + S2 + S3;

        if (p->bestS1 > S1) p->bestS1 = S1; // Si c'est son meilleur S1, on modifie le meilleur s1
        if (p->bestS2 > S2) p->bestS2 = S2; // Si c'est son meilleur S2, on modifie le meilleur s2
        if (p->bestS3 > S3) p->bestS3 = S3; // Si c'est son meilleur S3, on modifie le meilleur s3


        if (p->best > lap) p->best = lap; // Si c'est son meilleur temps au tour, on le notifie

        p->totalTime += lap; // Temps total

    } // Fin de la boucle

}

int main(int argc, char const *argv[]) {
    asciiArt(); // Because it's beautiful 

    // Variables pour la course
    int pilotes_numbers[MAX_PILOTES] = {44, 6, 5, 7, 3, 33, 19, 77, 11, 27, 26, 55, 14, 22, 9, 12, 20, 30, 8, 21, 31, 94}; // Tableau contenant les numéro des pilotes
    struct Pilote Q2[16]; // Tableau des pilotes lors de la Q2
    struct Pilote Q3[10]; // Tableau des pilotes lors de la Q3
    struct Pilote mainRun[MAX_PILOTES]; // Tableau des pilotes pour les autres séances

    // Variables de la shared memory
	int shmid = 0; // SH MEM id
    struct Pilote *pilotesTab; // Pointeur vers le tableau de pilotes

    // Variable du fork
    pid_t pid;
    pid_t tabPID[MAX_PILOTES]; // Tableau de pid

    /**
     * Mise en place de la shared memory
     */

    // Initialisation de la shared memory
	shmid = shmget(IPC_PRIVATE, MAX_PILOTES * sizeof(Pilote), IPC_CREAT | 0644); 

	if (shmid == -1) {
		perror("Erreur lors de l'allocation de la shared memory.");
		return 0;
	}

    // Attache la shared memory
	pilotesTab = shmat(shmid, NULL, 0);

    /* 
     * Les 7 événements de la course
     */

    for (int i = 1; i <= 7; i++) {

        switch(i) {
            case 1: // P1
            case 2: // P2
            case 3: // P3
                    //sem_post(&semaph); // sem + 1
                    for (int j = 0; j < MAX_PILOTES; j++) { /* Création des 22 processus */

                        tabPID[j] = fork();

                        if (tabPID[j] == -1) { // Erreur
                            printf("Erreur lors du fork()\n");
                            return 0;
                        }

                        if (tabPID[j] == 0) { // Fils
                            // http://stackoverflow.com/questions/8623131/why-is-rand-not-so-random-after-fork
                            srand(time(NULL) ^ (getpid() << 16));
                            pilotesTab[j].pilote_id = pilotes_numbers[j]; // Initialise le numéro du pilote
                            run(&pilotesTab[j], "Practices");
                            exit(0);
                 
                        } 
                    } /* Fin des 22 processus */

                    waitChildren(MAX_PILOTES);

                    printf("============================================================================================================== \n");
                    printf("P%d: \n", i);
                    fillTab(mainRun, pilotesTab, 0, MAX_PILOTES); // Remplis le tableau avec les données de la SM avant le tri + affichage
                    showResults(mainRun, MAX_PILOTES, "Practices");
                    printf("============================================================================================================== \n");
                   

                    break;
                case 4: // Q1
                    for (int j = 0; j < MAX_PILOTES; j++) { /* Création des 22 processus */

                        tabPID[j] = fork();

                        if (tabPID[j] == -1) { // Erreur
                            printf("Erreur lors du fork()\n");
                            return 0;
                        }

                        if (tabPID[j] == 0) { // Fils
                            pilotesTab[j].pilote_id = pilotes_numbers[j]; // Initialise le numéro du pilote
                            srand(time(NULL) ^ (getpid() << 16));
                            run(&pilotesTab[j], "Qualifs");
                            exit(0);
                        }

                    } /* Fin des 22 processus */

                    waitChildren(MAX_PILOTES);
                    printf("============================================================================================================== \n");
                    printf("Q1: \n");
                    fillTab(mainRun, pilotesTab, 0, MAX_PILOTES);
                    showResults(mainRun, MAX_PILOTES, "Qualifs");
                    printf("============================================================================================================== \n");

                    break;
                case 5: // Q2 => 16 voitures en piste
                    for (int i = 0; i < 16; i++) {
                        // mainRun a été trié
                        // trie le tableau de pilote_id
                        // avec les 15 premiers pilotes_id de Q1
                        pilotes_numbers[i] = mainRun[i].pilote_id;
                    }

                    for (int j = 0; j < 16; j++) { /* Création des 15 processus */

                        tabPID[j] = fork();

                        if (tabPID[j] == -1) { // Erreur
                            printf("Erreur lors du fork()\n");
                            return 0;
                        }

                        if (tabPID[j] == 0) { // Fils
                            srand(time(NULL) ^ (getpid() << 16));
                            pilotesTab[j].pilote_id = pilotes_numbers[j];
                            run(&pilotesTab[j], "Qualifs");
                            exit(0);
                        } 

                    } /* Fin des 22 processus */

                    waitChildren(16);
                    printf("============================================================================================================== \n");
                    printf("Q2: \n");
                    fillTab(Q2, pilotesTab, 0, 16);
                    showResults(Q2, 16, "Qualifs");
                    printf("============================================================================================================== \n");

                    break;
                case 6: // Q3 => 10 voitures en piste
                    for (int i = 0; i < 10; i++) {
                        pilotes_numbers[i] = Q2[i].pilote_id;
                    }

                    for (int j = 0; j < 10; j++) { /* Création des 10 processus */

                        tabPID[j] = fork();

                        if (tabPID[j] == -1) { // Erreur
                            printf("Erreur lors du fork()\n");
                            return 0;
                        }

                        if (tabPID[j] == 0) { // Fils
                            srand(time(NULL) ^ (getpid() << 16));
                            pilotesTab[j].pilote_id = pilotes_numbers[j];
                            run(&pilotesTab[j], "Qualifs");
                            exit(0);
                        } 

                    } /* Fin des 22 processus */

                    waitChildren(10);
                    printf("============================================================================================================== \n");
                    printf("Q3: \n");
                    fillTab(Q3, pilotesTab, 0, 10);
                    showResults(Q3, 10, "Qualifs");
                    printf("============================================================================================================== \n");

                    break;
                case 7: // Race

                    // Crée la grille de départ
                    for (int i = 0; i < 10; i++) {
                        // 10 premiers (Q3)
                        pilotes_numbers[i] = Q3[i].pilote_id;
                    }

                    for (int i = 10; i < 16; i++) {
                        // 6 suivants (Q2)
                        pilotes_numbers[i] = Q2[i].pilote_id;
                    }

                    for (int i = 16; i < MAX_PILOTES; i++) {
                        // Les derniers (Q1)
                        pilotes_numbers[i] = mainRun[i].pilote_id;
                    }

                    for (int j = 0; j < MAX_PILOTES; j++) { /* Création des 22 processus */

                        tabPID[j] = fork();

                        if (tabPID[j] == -1) { // Erreur
                            printf("Erreur lors du fork()\n");
                            return 0;
                        }

                        if (tabPID[j] == 0) { // Fils
                            srand(time(NULL) ^ (getpid() << 16));
                            pilotesTab[j].pilote_id = pilotes_numbers[j];
                            run(&pilotesTab[j], "Race");
                            exit(0);
                        }
                         
                    } /* Fin des 22 processus */

                    waitChildren(MAX_PILOTES);
                    printf("============================================================================================================== \n");
                    printf("Race: \n");
                    fillTab(mainRun, pilotesTab, 0, MAX_PILOTES);
                    showResults(mainRun, MAX_PILOTES, "Race");
                    printf("============================================================================================================== \n");

                    break;
        } 
    } /* fin des 7 événements de courses */

    shmdt(pilotesTab); // Détache la mémoire partagée de l'espace d'adresse du processus
    shmctl(shmid, IPC_RMID, 0); // Libère le segment de mémoire partagé

	return 0;
}

