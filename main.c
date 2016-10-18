#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
#include <string.h>

//#include "fonctions.h"

#define MAX_PILOTES 22
#define ID_PROJET 'FM1'
#define MAX_TOURS 44

  /**
    * Struct Pilote
    * Les temps sont en millisecondes
    * 1 struct pour toute les infos d'un pilote
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

int run(Pilote *p, char* name) {

	/* Instancie toutes les valeurs (exepté le pilote_id) à 0 */
	p->s1 = 0;
	p->bestS1 = 0;
	p->s2 = 0;
	p->bestS2 = 0;
	p->s3 = 0;
	p->bestS3 = 0;
	p->best = 0;
	p->isPit = 0;
	p->hasGivenUp = 0;
	p->numberOfPits = 0;

	for (int i = 0; i < MAX_TOURS; i++) { // Pour chaque tour

        p->isPit = 0; // Au début du tour, il n'est pas aux stands

  		if (!p->hasGivenUp) { // Si le pilote n'a pas abandonné

  			int temp1 = genRaceEvents();
  			int temp2 = genRaceEvents();
  			int temp3 = genRaceEvents();

  			p->hasGivenUp = genRaceEvents();

  			if (((temp1) && (temp2) && (temp3)) && strcmp(name, "Race") == 0) {
  				//printf("%s: Race\n", name);
  				//printf("We're in Race !!\n");
  				p->best = 3 * 60 * 3600;
				p->hasGivenUp = 1;
  			}
            
  			if (((temp1) && (temp2) && (temp3))) { // Si le pilote a abandonné, on s'arrête (on sort de la boucle)
  				p->hasGivenUp = 1;
                //printf("Le pilote %d a abandonné au tour %d\n", p->pilote_id, i+1);
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

        //else { // Sinon on peut faire un tour du circuit
        int S1 = genTime(30 * 3600, 35 * 3600);
        //printf("%d\n", S1/3600);
        int S2 = genTime(50 * 3600, 55 * 3600);
        int S3 = genTime(29 * 3600, 34 * 3600);

        if ((strcmp(name, "Race") == 0) && (p->isPit)) { // Si l'on est en course et que le pilote est au stand
            S1 += genTime(20 * 3600, 25 * 3600); // On rajoute entre 20 et 25sec au Secteur 1
        }

        p->s1 = S1; // On notifie le temps du S1
        p->s2 = S2; // On notifie le temps du S2
        p->s3 = S3; // etc...

        int lap = S1 + S2 + S3;

        if ((p->best == 0) || (p->bestS1 == 0) || (p->bestS2 == 0) || (p->bestS3 == 0)) { // Si l'on est au premier tours (en effet, au premier tours les bests temps sont initialisés à zéro !)
        	p->bestS1 = S1; 
        	p->bestS2 = S2; 
        	p->bestS3 = S3; 

        	p->best = lap;

        } else {

	        if (p->bestS1 > S1) p->bestS1 = S1; // Si c'est son meilleur S1, on modifie le meilleur s1
	        if (p->bestS2 > S2) p->bestS2 = S2; // Si c'est son meilleur S2, on modifie le meilleur s2
	        if (p->bestS3 > S3) p->bestS3 = S3; // Si c'est son meilleur S3, on modifie le meilleur s3


	        if (p->best > lap) p->best = lap; // Si c'est son meilleur temps au tour, on le notifie	
        }

        //printf("%d\n", lap);

        //if (i == MAX_TOURS - 1) exit(-1);

        // Problème avec p->best ==> Temps négatifs, temps de 0sec,...

	    //} 

    } // Fin de la boucle
}

void showResults(struct Pilote tab[], int nbElems) {

	/*for (int i = 0; i < nbElems; i++) {
		printf("%d\n", tab[i].best);
	}*/

	qsort(tab, nbElems, sizeof(Pilote), compare); 

    for (int k = 0; k<nbElems; k++) {
    	
        printf("%d%s%d%s%d%s%d%s%d%s%d%s\n" ,k+1,": voiture n°", tab[k].pilote_id,": ", tab[k].best/3600,"s (", tab[k].best/(60*3600),"m", (tab[k].best/3600)%60,"s", tab[k].best%60,"ms)"); 
    }
}

int main(int argc, char const *argv[]) {

    srand ((unsigned) time(NULL));

    int pilotes_numbers[MAX_PILOTES]  = {44, 6, 5, 7, 3, 33, 19, 77, 11, 27, 26, 55, 14, 22, 9, 12, 20, 30, 8, 21, 31, 94}; // Tableau contenant les numéro des pilotes

    struct Pilote pilotesTab[MAX_PILOTES]; // Tableau de structures pilote
    struct Pilote Q2[16]; // Tableau des pilotes lors de la Q2
    struct Pilote Q3[10]; // Tableau des pilotes lors de la Q3

    void fillTabBeforeRace() {
        for (int i = 0; i < 10; i++) {
            pilotesTab[i] = Q3[i];
        }

        for (int i = 10; i < 16; i++) {
            pilotesTab[i] = Q2[i];
        }
    }

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

 

    for (int i = 1; i <= 7; i++) {

           switch(i) {
               case 1:
               case 2:
               case 3:
                    for (int j = 0; j < MAX_PILOTES; j++) {

                    	//printf("%d\n", pilotes_numbers[j]);

                        pilotesTab[j].pilote_id = pilotes_numbers[j]; // Initialise le numéro du pilote

                        run(&pilotesTab[j], "Practices");

                        // Affichage
                        if (j == MAX_PILOTES - 1) {
                            printf("P%d:\n", i);

                            showResults(pilotesTab, MAX_PILOTES);

                        }
                    
                    }
                    break;
                case 4:
	                for (int j = 0; j < MAX_PILOTES; j++) {

                        run(&pilotesTab[j], "Qualifs");

                        // Affichage
                        if (j == MAX_PILOTES - 1) {
                            printf("Q1\n");

                            showResults(pilotesTab, MAX_PILOTES);

                            for (int k = 0; k<16; k++) {
                                Q2[k] = pilotesTab[k];
                            }
                        }
                    
                    }
                    break;
                case 5:
                	for (int j = 0; j < 16; j++) {

                        run(&pilotesTab[j], "Qualifs");

                        // Affichage
                        if (j == 15) {
                            printf("Q2\n");

              				showResults(Q2, 16);

                            for (int k = 0; k<10; k++) {
                                Q3[k] = Q2[k];
                            }
                        }
                    
                    }
                    break;
                case 6:
                     for (int j = 0; j < 10; j++) {

                        run(&pilotesTab[j], "Qualifs");

                        // Affichage
                        if (j == 9) {
                            printf("Q3\n");

                            showResults(Q3, 10);

                        }
                    
                    }
                    break;
                case 7:
					 fillTabBeforeRace(); // Crée la grille de départ

                     for (int j = 0; j < MAX_PILOTES; j++) {

                        run(&pilotesTab[j], "Race");

                        // Affichage
                        if (j == MAX_PILOTES - 1) {
                            printf("Race: \n");

                            showResults(pilotesTab, MAX_PILOTES);

                        }
                    
                    }
                    break;
                    
           } 

          

    }

	return 0;
}

