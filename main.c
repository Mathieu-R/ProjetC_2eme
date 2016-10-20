/* /!\ COMPILER AVEC -lm /!\ */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
#include <string.h>
#include <math.h>

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
    int hasGivenUpDuringRace;
    int numberOfPits;

} Pilote;

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
	if (result < 0) {return 0.0;}
	else {return result;}
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

int run(Pilote *p, char* name) {

	/* Instancie toutes les valeurs (exepté le pilote_id) à 0 */
	p->s1 = 3 * 60 * 3600 + 1;
	p->bestS1 =  3 * 60 * 3600 + 1;
	p->s2 =  3 * 60 * 3600 + 1;
	p->bestS2 =  3 * 60 * 3600 + 1;
	p->s3 =  3 * 60 * 3600 + 1;
	p->bestS3 =  3 * 60 * 3600 + 1;
	p->best =  3 * 60 * 3600 + 1;
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
  				//printf("%s: Race\n", name);
  				//printf("We're in Race !!\n");
  				p->best = 3 * 60 * 3600;
				p->hasGivenUp = 1;
				p->hasGivenUpDuringRace = 1;
  			}
            
  			if (((temp1) && (temp2) && (temp3))) { // Si le pilote a abandonné, on s'arrête (on sort de la boucle)
  				p->best = 3 * 60 * 3600 + 3;
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

        //printf("%d\t%d\t%d\n", S1, S2, S3);

        int lap = S1 + S2 + S3;

        if (p->bestS1 > S1) p->bestS1 = S1; // Si c'est son meilleur S1, on modifie le meilleur s1
        if (p->bestS2 > S2) p->bestS2 = S2; // Si c'est son meilleur S2, on modifie le meilleur s2
        if (p->bestS3 > S3) p->bestS3 = S3; // Si c'est son meilleur S3, on modifie le meilleur s3


        if (p->best > lap) p->best = lap; // Si c'est son meilleur temps au tour, on le notifie	

        //printf("%d\n", p->best);

    } // Fin de la boucle
}

void showResults(struct Pilote tab[], int nbElems) {

	qsort(tab, nbElems, sizeof(Pilote), compare); 

    for (int k = 0; k<nbElems; k++) {

    	//sleep(1);

    	if (tab[k].hasGivenUpDuringRace || tab[k].best == 3 * 60 * 3600 + 3) {
    		printf("voiture n°%d: Abandon\n", tab[k].pilote_id);
    	} else {
    		printf("%d%s%d%s%d%s%d%s%d%s\n" ,k+1,": voiture n°", tab[k].pilote_id,": (", tab[k].best/60000,"m", (tab[k].best/1000)%60,"s", tab[k].best-(tab[k].best/1000)*1000,"ms)"); 
    	}
    }
}

int main(int argc, char const *argv[]) {

    srand (time(NULL));

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
                    }

                    printf("P%d:\n", i);
                    showResults(pilotesTab, MAX_PILOTES);

                    break;
                case 4:
	                for (int j = 0; j < MAX_PILOTES; j++) {

                        run(&pilotesTab[j], "Qualifs");
                    
                    }

                    printf("Q1\n");
                    showResults(pilotesTab, MAX_PILOTES);

                    for (int k = 0; k<16; k++) {
                        Q2[k] = pilotesTab[k];
                    }
                    break;
                case 5:
                	for (int j = 0; j < 16; j++) {

                        run(&pilotesTab[j], "Qualifs");
                    
                    }

                    printf("Q2\n");
      				showResults(Q2, 16);

                    for (int k = 0; k<10; k++) {
                        Q3[k] = Q2[k];
                    }
                    break;
                case 6:
                     for (int j = 0; j < 10; j++) {

                        run(&pilotesTab[j], "Qualifs");
                    
                    }

                    printf("Q3\n");
                    showResults(Q3, 10);
                    break;
                case 7:
					 fillTabBeforeRace(); // Crée la grille de départ

                     for (int j = 0; j < MAX_PILOTES; j++) {

                        run(&pilotesTab[j], "Race");

                    }

                    printf("Race: \n");
                    showResults(pilotesTab, MAX_PILOTES);
                    break;
                    
           } 

          

    }

	return 0;
}

