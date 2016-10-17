/** transfer entre chaque Q.
* Concept rapide, a optimiser, doit être inplanté dans la main travaillée lundi.
*
**/
struct Pilote pilotesTab[MAX_PILOTES];
struct Pilote pilotesTabQ2[16];
struct Pilote pilotesTabQ3[10];
struct Pilote pilotesTabPositions[MAX_PILOTES];

Fin de main{
        /**
        * Fait les qualifs Q1, les trie et ensuite les fait entrer dans un tableau sur lequel peut être appliqué les qualifs Q2.
        * Devrait être fait sous forme de méthode qui return un tab de pilote prenant comme paramètres (pilotes[x], top) top étant le nombre de joueur sélectionés pour le prochain tour et donc la taille du nouveau tableau.
        * ce qui permetrait d'appeller la meme méthode pour chaque Q.
        **/
        for (int i=0; i < MAX_PILOTES; i++) makeQualifs(&pilotes[i]);

        qsort(pilotesTab, MAX_PILOTES, sizeof(Pilote), compare); 

        for (int i = 0; i<MAX_PILOTES; i++) {
        
            printf("%d%s%d%s%d%s%d%s%d%s\n" ,i+1,": voiture n°", pilotesTab[i].pilote_id,": ", pilotesTab[i].best,"s (", pilotesTab[i].best/60,"m", pilotesTab[i].best%60,"s)"); 
            if (i < 17) pilotesTabQ2[i] = pilotesTab[i];          
            else pilotesTabPositions[i] = pilotesTab[i]; // voir énoncé: * A la fin de Q1, il reste 16 voitures qualifiées pour Q2 et les 6 dernières sont placées à la fin de la grille de départ (places 17 à 22)
            // a ce point, pilotesTabQ2[16] est rempli par le top 16 du Q1 et est prêt a passer Q2.
            
            }
}
