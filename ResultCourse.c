#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define MAX_PILOTES 22

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

int compare(const void *p1, const void *p2) { // Méthode de comparation pour les temps
    const struct Pilote *elem1 = p1;
    const struct Pilote *elem2 = p2;

    if (elem1->best < elem2->best) return -1;
    if (elem1->best > elem2->best) return 1;
    return 0;
}

void showResults(struct Pilote tab[], int nbElems) {

	
	qsort(tab, nbElems, sizeof(Pilote), compare); 

    for (int k = 0; k < nbElems; k++) {

    	if (tab[k].hasGivenUpDuringRace || tab[k].best == 3 * 60 * 3600 + 3) {
    		printf("voiture n°%d: Abandon\n", tab[k].pilote_id);
    	} else {

    		printf(
    			"%d%s%d%s%d%s%d%s%d%s\n" ,k+1,
    			": voiture n°", tab[k].pilote_id,
    			": (", tab[k].best/60000,"m",
    			 (tab[k].best/1000)%60,"s",
    			  tab[k].best-(tab[k].best/1000)*1000,"ms)"
    			); 
    	}
    }
}

int main(int argc, char const *argv[])
{
	showResults(struct Pilote tab[], int nbElems);
	return 0;
}