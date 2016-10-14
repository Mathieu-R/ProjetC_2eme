#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{     
    // Création de la structure (devra etre importée depuis SM)
    int pool = 20; // nombre de voitures
    struct car 
    {
        int num;
        int s1;
    };
    struct car cars[pool]; // tab de structures - !!
    for (int i = 0; i<pool; i++) // remplissage du tableau avec des numéros de voiture génériques.
    {
        cars[i].num=i+1;
    }
    // Fin de la création du tableau de structure.
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
        cars[i].s1 = limitedRand(minTime, maxTime);
    }

    //Les boucles suivantes sont séparées car elles représentent des fonctions différentes du code final.

    for (int j = 0; j<pool; j++) // affichage la liste des temps (inutile dans le code final)
    {
        printf("%s%d%s%d%s%d%s%d%s\n" ,"voiture n°", cars[j].num,": ", cars[j].s1,"s (",cars[j].s1/60,"m",cars[j].s1%60,"s)"); // Le numéro de la première voiture du tableau est 1 (d'ou j+1)
    }
    for (int k = 0; k<pool; k++) // retient le meilleur temps et la voiture associée
    {
       if (cars[k].s1< currentBestTime)
        {
            currentBestTime = cars[k].s1;
            currentBestCar = cars[k].num; // Le numéro de la première voiture du tableau est 1 (d'ou k+1)
        } 
    }
    printf("%s%d%s%d%s%d%s\n" ,"La meilleure voiture est la voiture n°", currentBestCar," avec un temps de ", currentBestTime/60,"m",currentBestTime%60,"s"); // affiche la meilleure voiture et son temps
    
    return 0;
}
