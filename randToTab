#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int minTime = 600; // temps maximum en secondes
    int maxTime = 720; // temps minimum en secondes
    int cars[20]; // tab de temps.
    srand ( time(NULL) );
    
    int limitedRand(int low, int high) // Méthode de random basé sur min et max
    {
        return rand() % (high - low + 1) + low;
    }
    

    for (int i = 0; i<20; i++) // Remplissage du tableau
    {
        cars[i] = limitedRand(minTime, maxTime);
    }
    
    for (int j = 0; j<20; j++) // affichage (inutile dans le code final)
    {
        printf("%d\n " , cars[j]);
    }
    
    return 0;
}
