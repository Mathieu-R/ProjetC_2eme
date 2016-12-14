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
#include <semaphore.h>

#include "CourseF1.h"
#include "ResultCourse.h"

#define MAX_PILOTES 22

void showResults(struct Pilote tab[], int nbElems, char* name) {
    //sem_t semaph;

    //sem_wait(&semaph); // Attend que la fonction soit terminée avant que l'opération critique puisse commencée.'
    /*for (int i = 0; i < nbElems; i++) {
        // si le pilote a abandonné
        // au premier tour => pas de temps !
        if (tab[i].hasGivenUp && tab[i].best == 0) {
            // on fait en sorte qu'il soit à la fin du tableau 
            tab[i].best == (3 * 60 * 3600) + 1; 
        }
    }*/


    if (strcmp(name, "Race") != 0) { // NOT in Race
        qsort(tab, nbElems, sizeof(Pilote), compareBest);

        for (int k = 0; k < nbElems; k++) {

            // Si le pilote a abandonné durant la session
            // Mais a fait un temps
            if (tab[k].hasGivenUp) {
                printf(
                    "%d) voiture n°%d : Best S1 => %ds%dms | Best S2 => %ds%dms | Best S3 => %ds%dms | Best Lap => %dm%ds%dms || DNF\n", 
                    k+1,
                    tab[k].pilote_id, 
                    (tab[k].bestS1/1000)%60, tab[k].bestS1-(tab[k].bestS1/1000)*1000,
                    (tab[k].bestS2/1000)%60, tab[k].bestS2-(tab[k].bestS2/1000)*1000,
                    (tab[k].bestS3/1000)%60, tab[k].bestS3-(tab[k].bestS3/1000)*1000,
                    tab[k].best/60000, (tab[k].best/1000)%60, tab[k].best-(tab[k].best/1000)*1000
                );
                continue; 
            }

            // Si le pilote a abondonné au premier tour
            // HACK TIME pour le placer en fin de liste
            if (tab[k].hasGivenUp && tab[k].best == (3 * 60 * 3600) + 1) {
                printf("%d) voiture n°%d : // Abandon durant le premier tour de la session => Pas de temps // \n",
                    k+1,
                    tab[k].pilote_id
                );
                continue;
            }

            // Si tout s'est bien passé
            printf(
                "%d) voiture n°%d : Best S1 => %ds%dms | Best S2 => %ds%dms | Best S3 => %ds%dms | Best Lap => %dm%ds%dms \n", 
                k+1,
                tab[k].pilote_id, 
                (tab[k].bestS1/1000)%60, tab[k].bestS1-(tab[k].bestS1/1000)*1000,
                (tab[k].bestS2/1000)%60, tab[k].bestS2-(tab[k].bestS2/1000)*1000,
                (tab[k].bestS3/1000)%60, tab[k].bestS3-(tab[k].bestS3/1000)*1000,
                tab[k].best/60000, (tab[k].best/1000)%60, tab[k].best-(tab[k].best/1000)*1000
            ); 
        }

    } else { // IN race
        for (int k = 0; k < nbElems; k++) {
            qsort(tab, nbElems, sizeof(Pilote), compareTot);

            if (tab[k].hasGivenUpDuringRace) {
                printf("%d) voiture n°%d: DNF (n'a pas pu finir l'entiereté de la course pour cause d'abandon)\n",
                    k+1,
                    tab[k].pilote_id
                );
                continue;
            } 

            printf(
                "%d) voiture n°%d : Best S1 => %ds%dms | Best S2 => %ds%dms | Best S3 => %ds%dms | Best Lap => %dm%ds%dms || Total => %dm%ds%dms \n", 
                k+1,
                tab[k].pilote_id, 
                (tab[k].bestS1/1000)%60, tab[k].bestS1-(tab[k].bestS1/1000)*1000,
                (tab[k].bestS2/1000)%60, tab[k].bestS2-(tab[k].bestS2/1000)*1000,
                (tab[k].bestS3/1000)%60, tab[k].bestS3-(tab[k].bestS3/1000)*1000,
                tab[k].best/60000, (tab[k].best/1000)%60, tab[k].best-(tab[k].best/1000)*1000,
                tab[k].totalTime/60000, (tab[k].totalTime/1000)%60, tab[k].totalTime-(tab[k].totalTime/1000)*1000
            ); 
        }
    }

    //sem_post(&semaph);
}
