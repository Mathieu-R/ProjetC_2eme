#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int genTime(const int max) {
	return rand() % max + 1; // Generate number between 0 and max
}

int main(int argc, char const *argv[])
{
	key_t key = 5; // Clé
	size_t count = 1024; // Taille
	int perm = 0644; // Permissions

	int shmid = 0; // Shared Memory ID

	shmid = shmget(key, count, 0644);

	if (shmid == -1) {
		printf("ERREUR: BAD SHARED MEMORY ALLOCATION.");
		return 0;
	}
	
	return 0;
}