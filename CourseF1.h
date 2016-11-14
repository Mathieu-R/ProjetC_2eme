
typedef struct Pilote {
    //le numéro du pilote
    int pilote_id;
    //Les temps des différents secteurs
    int s1;
    int bestS1;
    int s2;
    int bestS2;
    int s3;
    int bestS3;

    // Meilleur temps pour le circuit complet
    int best;

    int isPit;
    int hasGivenUp;
    int numberOfPits;

} Pilote;

float ranf();
float randGaussien(float m, float s);
int genTime(const int min, const int max);
int genRaceEvents();
int compare(const void *p1, const void *p2);
int run (Pilote *p, char* name);
void fillTab(struct Pilote tabToFill[], struct Pilote tabFiller[], const int start, const int stop);