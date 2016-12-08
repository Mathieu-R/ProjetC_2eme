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



    if (strcmp(name, "Race") != 0) {
        qsort(tab, nbElems, sizeof(Pilote), compareBest);

        for (int k = 0; k < nbElems; k++) {

            if (tab[k].hasGivenUpDuringRace || tab[k].best == 3 * 60 * 3600 + 3) {
                printf("%d: voiture n°%d: Abandon\n", k+1, tab[k].pilote_id);
                continue;
            } 

            printf(
                "%d%s%d%s%d%s%d%s%d%s\n" ,k+1,
                ": voiture n°", tab[k].pilote_id,
                ": (", tab[k].best/60000,"m",
                    (tab[k].best/1000)%60,"s",
                    tab[k].best-(tab[k].best/1000)*1000,"ms)"
                ); 
        }
    } else {
        for (int k = 0; k < nbElems; k++) {
            qsort(tab, nbElems, sizeof(Pilote), compareTot);

            if (tab[k].hasGivenUpDuringRace || tab[k].best == 3 * 60 * 3600 + 3) {
                printf("voiture n°%d: Abandon\n", tab[k].pilote_id);
                continue;
            } 

            printf(
                "%d%s%d%s%d%s%d%s%d%s\n" ,k+1,
                ": voiture n°", tab[k].pilote_id,
                ": (", tab[k].totalTime/60000,"m",
                    (tab[k].totalTime/1000)%60,"s",
                    tab[k].totalTime-(tab[k].totalTime/1000)*1000,"ms)"
                ); 
        }
    }

    sem_post(&semaph);
}
