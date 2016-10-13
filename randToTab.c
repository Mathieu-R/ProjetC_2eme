#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{    
    int cars[pool]; // tab de temps - !! ce tableau correspond au contenu des structures voitures de la SM !! -
    int pool = 20; // nombre de voitures
    /**
    *  autre approche: définir un temps moyen et un taux de déviation par rapport à la moyenne.
    *  int average = 700; // moyenne de temps
    *  int deviation = 200; // difference entre average et extremes
    *  int minTime = average - deviation; // temps minimum en secondes
    *  int maxTime = average + deviation; // temps maximum en secondes
    **/
    int minTime = 500; // temps maximum en secondes
    int maxTime = 900; // temps minimum en secondes
    int cumulatedTime;
    int currentBestCar;
    int currentBestTime = maxTime + 1;
    int effectiveAverage;
    srand ( time(NULL) );
    
    int limitedRand(int low, int high) // Méthode de random basé sur min et max
    {
        return rand() % (high - low + 1) + low;
    }
    

    for (int i = 0; i<pool; i++) // Remplissage du tableau
    {
        cars[i] = limitedRand(minTime, maxTime);
    }
    
    for (int j = 0; j<pool; j++) // affichage la liste des temps (inutile dans le code final)
    {
        printf("%s%d%s%d%s\n" ,"voiture n°", j+1,": ", cars[j],"s"); // Le numéro de la première voiture du tableau est 1 (d'ou j+1)
        cumulatedTime =+ cars[j];
        
    }
    for (int k = 0; k<pool; k++) // retient le meilleur temps et la voiture associée
    {
       if (cars[k]< currentBestTime)
        {
            currentBestTime = cars[k];
            currentBestCar = k+1; // Le numéro de la première voiture du tableau est 1 (d'ou k+1)
        } 
    }
    printf("%s%d%s%d%s\n" ,"La meilleure voiture est la voiture n°", currentBestCar," avec un temps de ", currentBestTime,"s"); // affiche la meilleure voiture et son temps
    
    return 0;
}
