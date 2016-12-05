/* /!\ COMPILER AVEC -lm && -lpthread /!\ */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <semaphore.h>

#include "CourseF1.h"
#include "ResultCourse.h"

#define MAX_PILOTES 22
#define MAX_TOURS 44

float ranf() { // PRNG pour des floats [0, 1].
	float r = rand() / (float) RAND_MAX;
	return r;
}

/**
*
* Cette méthode est basée sur la transformation de Box-Muller: https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
*
**/
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

int genRaceEvents() { // Décide des faits de courses
	return rand() % 2; // On génère le nombre, soit 0, soit 1;
}

int compare(const void *p1, const void *p2) { // Méthode de comparation pour les temps
    const struct Pilote *elem1 = p1;
    const struct Pilote *elem2 = p2;

    if (elem1->best < elem2->best) return -1;
    if (elem1->best > elem2->best) return 1;
    return 0;
}

void fillTab(struct Pilote tabToFill[], struct Pilote tabFiller[], const int start, const int stop) {
    for (int i = start; i < stop; i++) {
        tabToFill[i] = tabFiller[i];
    }
}

int run(Pilote *p, char* name) {

    /* Instancie toutes les valeurs (exepté le pilote_id) à 0 */
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

        if (!p->hasGivenUp) { // Si le pilote n'a pas abandonné

            int temp1 = genRaceEvents();
            int temp2 = genRaceEvents();
            int temp3 = genRaceEvents();

            p->hasGivenUp = genRaceEvents();

            if (((temp1) && (temp2) && (temp3)) && strcmp(name, "Race") == 0) {
                p->best = 3 * 60 * 3600;
                p->hasGivenUp = 1;
                p->hasGivenUpDuringRace = 1;
            }
            
            if (((temp1) && (temp2) && (temp3))) { // Si le pilote a abandonné, on s'arrête (on sort de la boucle)
                p->best = 3 * 60 * 3600 + 3;
                p->hasGivenUp = 1;
                return 0;
            }
        }

        if (p->numberOfPits < 2) { // Max 2 arrêts
            p->isPit = genRaceEvents();

            if (p->isPit) {
                p->numberOfPits++;
                if ((strcmp(name, "Practices") == 0)|| (strcmp(name, "Qualifs") == 0)) continue; // On passe à l'itération suivante
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

        if ((strcmp(name, "Race") == 0)) {
            p->totalTime += lap;
        }	

    } // Fin de la boucle
	
}

int main(int argc, char const *argv[]) {

    srand (time(NULL)); // Utile pour la génération de nombre aléatoire

    // Variables pour la course
    int pilotes_numbers[MAX_PILOTES]  = {44, 6, 5, 7, 3, 33, 19, 77, 11, 27, 26, 55, 14, 22, 9, 12, 20, 30, 8, 21, 31, 94}; // Tableau contenant les numéro des pilotes
    struct Pilote Q2[16]; // Tableau des pilotes lors de la Q2
    struct Pilote Q3[10]; // Tableau des pilotes lors de la Q3
    struct Pilote mainRun[MAX_PILOTES]; // Tableau des pilotes pour les autres séances

    // Variables de la shared memory
	key_t key; // Clé
	int shmid; // SH MEM id
    struct Pilote *pilotesTab; // Pointeur vers le tableau de pilotes
    pid_t tabPID[MAX_PILOTES]; // Tableau de pid

    // Variable du sémaphore
    sem_t semaph; // Sémaphore

    // Variable du fork
    pid_t pid;

    /**
     * Mise en place de la shared memory
     */
    
    // Génération de la clé pour la shared memory
    key = ftok(argv[0], 123); // argv[O] => nom du programme lancé, ID (char)

    // Initialisation de la shared memory
	shmid = shmget(key, MAX_PILOTES * sizeof(Pilote), IPC_CREAT | 0644); 

	if (shmid == -1) {
		printf("Erreur lors de l'allocation de la shared memory.");
		return 0;
	}

    // Attache la shared memory
	pilotesTab = shmat(shmid, NULL, 0);

    /**
     * Mise en place des sémaphores
     */

    // initialisation du sémaphore
    if (sem_init(&semaph, 1, 1) == -1) { // pointeur vers le sémaphore, 0 => sémaphore partagés entre les threads, valeur initiale du semaphore
        printf("Erreur: erreur lors de l'initialisation du sémaphore'");
        return 0;
    }


    /* 
     * Les 7 événements de la course
     */

    for (int i = 1; i <= 7; i++) {

        switch(i) {
            case 1: // P1
            case 2:  // P2
            case 3: // P3
                    for (int j = 0; j < MAX_PILOTES; j++) { /* Création des 22 processus */

                        tabPID[j] = fork();

                        if (tabPID[j] == -1) { // Erreur
                            printf("Erreur lors du fork()\n");
                            return 0;
                        }

                        if (tabPID[j] == 0) { // Fils

                            pilotesTab[j].pilote_id = pilotes_numbers[j]; // Initialise le numéro du pilote
                            run(&pilotesTab[j], "Practices");
                            exit(0);
                 
                        } 

                    } /* Fin des 22 processus */
                    
                    printf("P%d\n", i);
                    fillTab(mainRun, pilotesTab, 0, MAX_PILOTES); // Remplis le tableau avec les données de la SM avant le tri + affichage
                    sem_post(&semaph); // Indique si la fonction est terminée, on peut donc faire l'opération critique'
                    showResults(mainRun, MAX_PILOTES);

                   

                    break;
                case 4: // Q1
                    for (int j = 0; j < MAX_PILOTES; j++) { /* Création des 22 processus */

                        tabPID[j] = fork();

                        if (tabPID[j] == -1) { // Erreur
                            printf("Erreur lors du fork()\n");
                            return 0;
                        }

                        if (tabPID[j] == 0) { // Fils
                            run(&pilotesTab[j], "Qualifs");
                            exit(0);
                        }

                    } /* Fin des 22 processus */

                    printf("Q1\n");
                    fillTab(mainRun, pilotesTab, 0, MAX_PILOTES);
                    sem_post(&semaph);
                    showResults(mainRun, MAX_PILOTES);

                    fillTab(Q2, mainRun, 0, 10); // Remplis le tableau de Q2 avec les 10 premiers de la Q1

                    break;
                case 5: // Q2
                    for (int j = 0; j < MAX_PILOTES; j++) { /* Création des 22 processus */

                        tabPID[j] = fork();

                        if (tabPID[j] == -1) { // Erreur
                            printf("Erreur lors du fork()\n");
                            return 0;
                        }

                        if (tabPID[j] == 0) { // Fils
                            run(&pilotesTab[j], "Qualifs");
                            exit(0);
                        } 

                    } /* Fin des 22 processus */

                    printf("Q2\n");
                    sem_post(&semaph);
                    showResults(Q2, 16);

                    fillTab(Q3, Q2, 0, 10); // Remplis le tableau de Q3 avec les 10 premiers de la Q2

                    break;
                case 6: // Q3
                    for (int j = 0; j < MAX_PILOTES; j++) { /* Création des 22 processus */

                        tabPID[j] = fork();

                        if (tabPID[j] == -1) { // Erreur
                            printf("Erreur lors du fork()\n");
                            return 0;
                        }

                        if (tabPID[j] == 0) { // Fils
                            run(&pilotesTab[j], "Qualifs");
                            exit(0);
                        } 

                    } /* Fin des 22 processus */

                    printf("Q3\n");
                    showResults(Q3, 10);

                    break;
                case 7: // Race

                    // Crée la grille de départ
                    fillTab(mainRun, Q3, 0, 10); // Remplis les 10 premiers de la Q3
                    fillTab(mainRun, Q2, 10, 16); // Remplis les 6 suivants de la Q2

                    for (int j = 0; j < MAX_PILOTES; j++) { /* Création des 22 processus */

                        tabPID[j] = fork();

                        if (tabPID[j] == -1) { // Erreur
                            printf("Erreur lors du fork()\n");
                            return 0;
                        }

                        if (tabPID[j] == 0) { // Fils
                            run(&pilotesTab[j], "Race");
                            exit(0);
                        }
                         
                    } /* Fin des 22 processus */

                    printf("Race: \n");
                    fillTab(mainRun, pilotesTab, 0, MAX_PILOTES);
                    sem_post(&semaph);
                    showResults(mainRun, MAX_PILOTES);

                    break;
        } 
    } /* fin des 7 événements de courses */

    sem_destroy(&semaph); // Détruit le sémaphore
    shmdt(pilotesTab); // Détache la mémoire partagée
    shmctl(shmid, IPC_RMID, 0); // Libère la mémoire partagé

	return 0;
}

