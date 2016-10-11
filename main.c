#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_PILOTES 22

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


    /**
     * Struct Pilote
     * Les temps sont en millisecondes
     * 1 struct par séance
     * S => Secteur
     * best => Meilleur temps
     */

    typedef struct Pilote {

        int pilote_id;

        typedef struct Essais {
                typedef struct S1 {
                    int S1;
                    int bestS1;
                } S1;
                typedef struct S2 {
                    int S2;
                    int bestS2;
                } S2;
                typedef struct S3 {
                    int S3;
                    int bestS3;
                } S3;
                int best; // Best Full Time Lap
                bool isPit;
                bool isOut;
        } Essais;

        typedef struct Qualifs {
            typedef struct S1 {
                int S1;
                int bestS1;
            } S1;
            typedef struct S2 {
                int S2;
                int bestS2;
            } S2;
            typedef struct S3 {
                int S3;
                int bestS3;
            } S3;
            int best; // Best Full Time Lap
            bool isPit;
            bool isOut;
        } Qualifs;

        typedef struct Course {
            typedef struct S1 {
                int S1;
                int bestS1;
            } S1;
            typedef struct S2 {
                int S2;
                int bestS2;
            } S2;
            typedef struct S3 {
                int S3;
                int bestS3;
            } S3;
            int best; // Best Full Time Lap
            bool isPit;
            bool isOut;
        } Course;

    } Pilote;



	key_t key = 5; // Clé
	size_t count = 1024; // Taille
	int perm = 0644; // Permissions

	int shmid = 0; // Shared Memory ID

	shmid = shmget(key, count, 0644);

	if (shmid == -1) {
		printf("ERREUR: BAD SHARED MEMORY ALLOCATION.");
		return 0;
	}

    for (int i=0; i < MAX_PILOTES; i++) {

        if (fork() == 0) {
            return 0;
        }

    }
	
	return 0;
}