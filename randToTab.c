#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{     
    // Création de la structure (devra etre importée depuis SM)
    int pool = 20; // nombre de voitures (obtenir cette valeur depuis la SM si possible)
    typedef struct car 
    {
        int num;
        int s1;
    }car;
    struct car cars[pool]; // tab de structures - !!
    for (int i = 0; i<pool; i++) // rempli la structure pour tester le code (sera importé de la SM dans le code final)
    {
        cars[i].num=i+1;
    }
    /**
    *  autre approche: définir un temps moyen et un taux de déviation par rapport à la moyenne.
    *  int average = 700; // moyenne de temps
    *  int deviation = 200; // difference entre average et extremes
    *  int minTime = average - deviation; // temps minimum en secondes
    *  int maxTime = average + deviation; // temps maximum en secondes
    **/
    int minTime = 500; // temps maximum en secondes
    int maxTime = 900; // temps minimum en secondes
    int currentBestCar;
    int currentBestTime = maxTime + 1;
    srand ( time(NULL) );
    
// --------------- Méthodes -------------
    int limitedRand(int low, int high) // Méthode de random basé sur min et max
    {
        return rand() % (high - low + 1) + low;
    }
    int compare(const void *p1, const void *p2) // Méthode de comparation pour car cars[].s1
    {
        const struct car *elem1 = p1;    
        const struct car *elem2 = p2;
    
       if ( elem1->s1 < elem2->s1)
          return -1;
       else if (elem1->s1 > elem2->s1)
          return 1;
       else
          return 0;
    }
// -------------- Calculs ---------------
    for (int i = 0; i<pool; i++) // Remplissage du tableau
    {
        cars[i].s1 = limitedRand(minTime, maxTime);
    }
    for (int k = 0; k<pool; k++) // retient le meilleur temps et la voiture associée
    {
       if (cars[k].s1< currentBestTime)
        {
            currentBestTime = cars[k].s1;
            currentBestCar = cars[k].num; 
        } 
    }    

// -------------- Affichage --------------

    for (int j = 0; j<pool; j++) // affichage la liste des temps (inutile dans le code final)
    {
        printf("%s%d%s%d%s%d%s%d%s\n" ,"voiture n°", cars[j].num,": ", cars[j].s1,"s (",cars[j].s1/60,"m",cars[j].s1%60,"s)");
    }
    
    printf("\n%s%d%s%d%s%d%s\n\n" ,"La meilleure voiture est la voiture n°", currentBestCar," avec un temps de ", currentBestTime/60,"m",currentBestTime%60,"s"); // affiche la meilleure voiture et son temps

    qsort(cars, pool, sizeof(car), compare); 
    for (int j = 0; j<pool; j++) // affichage la liste des temps classé.
    {
        printf("%d%s%d%s%d%s%d%s%d%s\n" ,j+1,": voiture n°", cars[j].num,": ", cars[j].s1,"s (",cars[j].s1/60,"m",cars[j].s1%60,"s)"); 
    }

    return 0;
}
