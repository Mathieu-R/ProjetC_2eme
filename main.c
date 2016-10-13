#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>

#define MAX_PILOTES 22
#define ID_PROJET 'P'
#define MAX_TOURS 44

int genTime(const int min, const int max) {
    srand (time(NULL));
    return ((rand() % (max-min)) + min); // Generate number between min and max
}

int main(int argc, char const *argv[])
{
    
    /**
     * 1 processus par pilote !!
     * Fork
     */
    
    typedef struct S1 {
        int S1; // Temps S1
        int bestS1; // Meilleur S1
    } S1;

    typedef struct S2 {
        int S2; // Temps S2
        int bestS2; // Meilleur S2
    } S2;

    typedef struct S3 {
        int S3; // Temps S3
        int bestS3; // Meilleur S3
    } S3;

	typedef struct Essais {
        
        struct S1;
        struct S2;
        struct S3;

        int best; // Best Full Time Lap
        bool isPit; // Si le pilote est aux stands
        bool isOut; // Si le pilote a abandonné
    } Essais;

    typedef struct Qualifs {
        
        struct S1;
        struct S2;
        struct S3;

        int best; // Best Full Time Lap
        bool isPit;
        bool isOut;
    } Qualifs;


    typedef struct Course {
        
        struct S1;
        struct S2;
        struct S3;

        int best; // Best Full Time Lap
        bool isPit;
        bool isOut;
    } Course;

    /**
     * Struct Pilote
     * Les temps sont en millisecondes
     * 1 struct par séance
     * S => Secteur
     * best => Meilleur temps
     */

    typedef struct Pilote {

        int pilote_id;

        struct Essais; // Struct pour les Essais
        struct Qualifs; // Struct pour les Qualifs
        struct Course; // Struct pour la Course
      

    } Pilote;



	/*key_t key; // Clé
    key = ftok(argv[0], ID_PROJET); // argv[O] => nom du programme lancé, ID (char)
	int perm = 0644; // Permissions

	int shmid = 0; // Shared Memory ID

	shmid = shmget(key, sizeof(Pilote), 0666);

	if (shmid == -1) {
		printf("ERREUR: BAD SHARED MEMORY ALLOCATION.");
		return 0;
	}*/

    for (int i=0; i < MAX_PILOTES; i++) {

        if (fork() == 0) {

			for (int i = 0; i < MAX_TOURS; i++) {



			}			            

        }

    }
	
	return 0;
}