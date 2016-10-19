/** 
* !!! -- A LIRE -- !!!
* Le fichier doit être compilé de cette manière " gcc -o main *.c -lm " (le -lm est important pour faire fonctionner sqrt et log).
* l'écart type n'est pas une limite dure; Voir https://fr.wikipedia.org/wiki/%C3%89cart_type
* -> Pour forcer un temps minimum, ils nous suffira d'additioner le temps au résultat de la méthode comme: "5 minutes + randGaussien"
* Les méthodes à importer dans le code sont: float ranf() et float randGaussien(...).
* Ne pas oublier d'importer <math.h> .
**/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main()
{
    srand ( time(NULL) );
    int a;


    float ranf() // PRNG pour des floats [0, 1].
    {
          float r = rand() / (float) RAND_MAX;
          return r;
    }

    /**
    *
    * Cette méthode est basée sur la transformation de Box-Muller: https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
    *
    **/
    float randGaussien(float m, float s)	/* PRNG distribution normale (gaussienne) */
    {				        /* median m, écart-type s */
    	float x1, x2, w, y1;
    	static float y2;
    	static int use_last = 0;
    
    	if (use_last)		        /* utilise la valeur de l'appel précédent */
    	{
    		y1 = y2;
    		use_last = 0;
    	}
    	else
    	{
    		do {
    			x1 = 2.0 * ranf() - 1.0;
    			x2 = 2.0 * ranf() - 1.0;
    			w = x1 * x1 + x2 * x2;
    		} while ( w >= 1.0 );
    
    		w = sqrt( (-2.0 * log( w ) ) / w );
    		y1 = x1 * w;
    		y2 = x2 * w;
    		use_last = 1;
    	}
    
    	return( m + y1 * s );
    }
            
        for (int i = 0; i<10; i++) // Affichage des résultats du PRNG a titre d'exemple
        {
            a = (int) randGaussien(1000.0, 50.0);
            printf("%d\n", a);

        }
    
        return 0;
}
