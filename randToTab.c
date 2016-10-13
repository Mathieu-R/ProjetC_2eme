#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int pool = 20; // nombre de voitures
    /* autre approche: définir un temps moyen et un taux de déviation par rapport à la moyenne.
    *  int average = 700; // moyenne de temps
    *  int deviation = 200; // difference entre average et extremes
    *  int minTime = average - deviation; // temps minimum en secondes
    *  int maxTime = average + deviation; // temps maximum en secondes
    */
    int minTime = 500; // temps maximum en secondes
    int maxTime = 900; // temps minimum en secondes
    int cars[pool]; // tab de temps.
    srand ( time(NULL) );
    
    int limitedRand(int low, int high) // Méthode de random basé sur min et max
    {
        return rand() % (high - low + 1) + low;
    }
    

    for (int i = 0; i<pool; i++) // Remplissage du tableau
    {
        cars[i] = limitedRand(minTime, maxTime);
    }
    
    for (int j = 0; j<pool; j++) // affichage (inutile dans le code final)
    {
        printf("%s%d%s%d%s\n " ,"voiture n°", j+1,": ", cars[j],"s");
    }
    
    return 0;
}
