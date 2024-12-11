#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct neur
{
    int *poids;
    int seuil;
    int nombre_entrees;
    struct neur *suivant;
    struct neur *precedent;

} Neurone;

typedef struct couche
{
    int nombre_neurones;
    Neurone *debutN;
    Neurone *finN;
    struct couche *suivante;
    struct couche *precedente;
} Coucheneurone;

typedef struct
{
    int nombre_couche;
    Coucheneurone *finC;
    Coucheneurone *debutC;
} Reseau;

Neurone *InitNeurone(int *poids, int seuil, int n_Entry)
{
    Neurone *neurone = (Neurone *)malloc(sizeof(Neurone));
    neurone->nombre_entrees = n_Entry;
    neurone->poids = (int *)malloc(n_Entry * sizeof(int));

    for (int i = 0; i < n_Entry; i++) // Initialise les poids du neurone
    {
        neurone->poids[i] = poids[i];
    }
    neurone->seuil = seuil;
    neurone->suivant = NULL;
    neurone->precedent = NULL;

    return neurone;
}

Coucheneurone *InitCouche(int nombre_neurones, int n_Entry, int **poids, int *seuil)
{
    Coucheneurone *couche = (Coucheneurone *)malloc(sizeof(Coucheneurone));
    couche->nombre_neurones = nombre_neurones;
    Neurone *neurone = NULL;

    for (int j = 0; j < nombre_neurones; j++) // Initialise chaque neurone
    {
        Neurone *new = InitNeurone(poids[j], seuil[j], n_Entry);
        if (j == 0) // cas particulier tete de la liste chainée
        {
            couche->debutN = new;
            new->precedent = NULL;
        }
        else
        {
            neurone->suivant = new;
            new->precedent = neurone;
        }
        neurone = new;
    }
    couche->finN = neurone;
    couche->suivante = NULL;
    couche->precedente = NULL;
    return couche;
}

int OutNeurone(Neurone *neurone, int *Entry)
{
    int valeur = 0;

    for (int i = 0; i < neurone->nombre_entrees; i++) // calcule la somme pondérer
    {
        valeur = valeur + neurone->poids[i] * Entry[i];
    }

    if (valeur >= neurone->seuil) // test le résultat par rapport au seuil
    {
        return 1;
    }
    return 0;
}

int *OutCouche(Coucheneurone *couche, int *Entry)
{
    int *sorties = (int *)malloc(couche->nombre_neurones * sizeof(int));
    Neurone *tempN = NULL;

    for (int i = 0; i < couche->nombre_neurones; i++) // Passe dans chaque neurone de la couche
    {
        if (i == 0)
        {
            tempN = couche->debutN;
        }
        sorties[i] = OutNeurone(tempN, Entry);
        tempN = tempN->suivant;
    }
    return sorties;
}

Reseau CreateR(int nombrecouches, int *Nneurone, int n_Entry, int ***poids, int **seuil)
{
    Reseau R;
    Coucheneurone *precedente = NULL;

    for (int h = 0; h < nombrecouches; ++h)
    {
        Coucheneurone *newcouche = InitCouche(Nneurone[h], n_Entry, poids[h], seuil[h]);
        if (h == 0)
        {
            R.debutC = newcouche;
        }
        else
        {
            precedente->suivante = newcouche;
            newcouche->precedente = precedente;
        }

        precedente = newcouche;
        n_Entry = Nneurone[h];
    }

    R.finC = precedente;

    return R;
}

int *OutR(Reseau R, int *Entry, int nombrecouches)
{
    int *sorties = Entry;

    Coucheneurone *couche = R.debutC;
    for (int i = 0; i < nombrecouches; i++) // passe dans chaque couche
    {
        sorties = OutCouche(couche, sorties);
        couche = couche->suivante;
    }
    return sorties;
}

void *Choix_Entry(int Nentrer, int *Entry)
{
    char reponse;
    int choix = 0;

    if (Nentrer == 0)
    {
        do
        {
            printf("\n");
            printf("-------------------------------------------------\n");
            printf("- Nombre de entrer dans le reseau :             -\n");
            printf("-------------------------------------------------\n");
            scanf("%s", &reponse);
            Nentrer = strtol(&reponse, NULL, 10);
        } while (Nentrer < 0);
    }
    for (int i = 0; i < Nentrer; i++)
    {
        choix = -1;
        reponse = 'a';
        do
        {
            printf("\n");
            printf("-------------------------------------------------\n");
            printf("-Choix entrer %d :                              -\n", i);
            printf("-------------------------------------------------\n");
            scanf("%s", &reponse);
            choix = strtol(&reponse, NULL, 10);
        } while (choix != 0 && choix != 1);
        Entry[i] = choix;
    }
}

int menu()
{
    char reponse;
    int choixmenu = 0;
    int couche = 0;
    int *index = 0;
    do
    {
        printf("\n");
        printf("-------------------------------------------------\n");
        printf("- Actions disponibles :                         -\n");
        printf("-------------------------------------------------\n");
        printf("-[1] - Neurone AND                              -\n");
        printf("-[2] - Neurone OR                               -\n");
        printf("-[3] - Neurone NOT                              -\n");
        printf("-[4] - (A ET (non B) ET C) OU (A ET (non C))    -\n");
        printf("-[5] - expression                               -\n");
        printf("-------------------------------------------------\n");
        printf("\nQue souhaitez vous faire ? [1/2/3/4]\n");
        scanf("%c", &reponse);
        choixmenu = strtol(&reponse, NULL, 10);
        switch (choixmenu)
        {
        case 1:
        {
            int nombrecouches = 1;
            int listenombre[] = {1}; // Nombre de neurone par couches
            int Nentry = 2;

            // initalisation Entrer
            int Entry[Nentry];
            Choix_Entry(Nentry, Entry);

            // Initialisation des poids et seuils
            int poids1[] = {1, 1};
            int *poidsCouche1[] = {poids1};
            int **poids[] = {poidsCouche1};

            int seuilcouche1[] = {Nentry};
            int *seuil[] = {seuilcouche1};

            Reseau R = CreateR(nombrecouches, listenombre, Nentry, poids, seuil); // initialise le Reseau
            int *sortie = OutR(R, Entry, nombrecouches);                          // Calcule le résultat

            printf("Sortie pour A=%d B=%d de l'expression logique : A AND B est : %d\n", Entry[0], Entry[1], sortie[0]);
            break;
        }
        case 2:
        {
            int nombrecouches = 1;
            int listenombre[] = {1}; // Nombre de neurone par couches
            int Nentry = 2;

            // initalisation Entrer
            int Entry[Nentry];
            Choix_Entry(Nentry, Entry);

            // Initialisation des poids et seuils
            int poids1[] = {1, 1};
            int *poidsCouche1[] = {poids1};
            int **poids[] = {poidsCouche1};

            int seuilcouche1[] = {1};
            int *seuil[] = {seuilcouche1};

            Reseau R = CreateR(nombrecouches, listenombre, Nentry, poids, seuil); // initialise le Reseau
            int *sortie = OutR(R, Entry, nombrecouches);                          // Calcule le résultat

            printf("Sortie pour A=%d B=%d de l'expression logique : A OR B est : %d\n", Entry[0], Entry[1], sortie[0]);
            break;
        }
        case 3:
        {
            int nombrecouches = 1;
            int listenombre[] = {1}; // Nombre de neurone par couches
            int Nentry = 1;

            // initalisation Entrer
            int Entry[Nentry];
            Choix_Entry(Nentry, Entry);

            // Initialisation des poids et seuils
            int poids1[] = {-1};
            int *poidsCouche1[] = {poids1};
            int **poids[] = {poidsCouche1};

            int seuilcouche1[] = {0};
            int *seuil[] = {seuilcouche1};

            Reseau R = CreateR(nombrecouches, listenombre, Nentry, poids, seuil); // initialise le Reseau
            int *sortie = OutR(R, Entry, nombrecouches);                          // Calcule le résultat

            printf("Sortie pour A=%d de l'expression logique : !A est : %d\n", Entry[0], sortie[0]);
            break;
        }
        case 4:
        {
            int nombrecouches = 3;
            int listenombre[] = {4, 2, 1}; // Nombre de neurone par couches
            int Nentry = 3;

            // initalisation Entrer
            int Entry[Nentry];
            Choix_Entry(Nentry, Entry);

            // Initialisation des poids et seuils
            int poids1[] = {1, 0, 0};  // Poids du neurone 1 de la couche 1
            int poids2[] = {0, -1, 0}; // Poids du neurone 2 de la couche 1
            int poids3[] = {0, 0, 1};  // Poids du neurone 3 de la couche 1
            int poids4[] = {0, 0, -1}; // Poids du neurone 4 de la couche 1

            int poids5[] = {1, 1, 1, 0}; // Poids du neurone 1 de la couche 2
            int poids6[] = {1, 0, 0, 1}; // Poids du neurone 2 de la couche 2

            int poids7[] = {1, 1}; // Poids du neurone de la couche 3

            // Définir les poids pour chaque couche
            int *poidsCouche1[] = {poids1, poids2, poids3, poids4};
            int *poidsCouche2[] = {poids5, poids6};
            int *poidsCouche3[] = {poids7};

            // Déclarer un tableau de pointeurs vers les couches
            int **poids[] = {poidsCouche1, poidsCouche2, poidsCouche3};

            int seuilcouche1[] = {1, 0, 1, 0}; // Seuils des neurones de la couche 1
            int seuilcouche2[] = {3, 2};       // Seuils des neurones de la couche 2
            int seuilcouche3[] = {1};
            int *seuil[] = {seuilcouche1, seuilcouche2, seuilcouche3};

            Reseau R = CreateR(nombrecouches, listenombre, Nentry, poids, seuil); // initialise le Reseau
            int *sortie = OutR(R, Entry, nombrecouches);                          // Calcule le résultat

            printf("Sortie pour A=%d, B=%d, C=%d de l'expression logique : (A ET (!B) ET C) OU (A ET (!C)) est : %d\n", Entry[0], Entry[1], Entry[2], sortie[0]);
            break;
        }
        case 5:
        {
            int nombrecouches = 0;
            do
            { // couche
                printf("\n");
                printf("-------------------------------------------------\n");
                printf("- nombre de couches (min 1):                    -\n");
                printf("-------------------------------------------------\n");
                scanf("%s", &reponse);
                nombrecouches = strtol(&reponse, NULL, 10);
            } while (nombrecouches < 1);

            int *listenombre = malloc(nombrecouches * sizeof(int));
            int choix;

            for (int i = 0; i < nombrecouches; i++) // neurone par couche
            {
                choix = -1;
                reponse = 'a';
                do
                {
                    printf("\n");
                    printf("-------------------------------------------------\n");
                    printf("- Nombre de neurone dans la couche %d :         -\n", i + 1);
                    printf("-------------------------------------------------\n");
                    scanf("%s", &reponse);
                    choix = strtol(&reponse, NULL, 10);
                } while (choix < 1);
                listenombre[i] = choix;
            }

            int nEntry = 0;
            do // nombre entrer
            {
                printf("\n");
                printf("-------------------------------------------------\n");
                printf("- Nombre d'entrer (min 1):                       -\n");
                printf("-------------------------------------------------\n");
                scanf("%s", &reponse);
                nEntry = strtol(&reponse, NULL, 10);
            } while (nEntry < 1);

            int ***poids = malloc(nombrecouches * sizeof(int **));
            for (int j = 0; j < nombrecouches; ++j) // poids
            {
                poids[j] = malloc(listenombre[j] * sizeof(int *));
                for (int i = 0; i < listenombre[j]; ++i)
                {

                    choix = -1;
                    reponse = 'a';
                    if (j == 0)
                    {
                        poids[j][i] = malloc(nEntry * sizeof(int));
                        for (int k = 0; k < nEntry; ++k)
                        {
                            do
                            {
                                printf("\n");
                                printf("-------------------------------------------------\n");
                                printf("- Poid %d du neurone %d dans la couche %d :     -\n", k + 1, i + 1, j + 1);
                                printf("-------------------------------------------------\n");
                                scanf("%s", &reponse);
                                choix = strtol(&reponse, NULL, 10);
                            } while (choix != 0 && choix != 1 && choix != -1);
                            poids[j][i][k] = choix;
                        }
                    }
                    else
                    {
                        poids[j][i] = malloc(listenombre[j - 1] * sizeof(int));
                        for (int k = 0; k < listenombre[j - 1]; ++k)
                        {
                            do
                            {
                                printf("\n");
                                printf("-------------------------------------------------\n");
                                printf("- Poid %d du neurone %d dans la couche %d :     -\n", k + 1, i + 1, j + 1);
                                printf("-------------------------------------------------\n");
                                scanf("%s", &reponse);
                                choix = strtol(&reponse, NULL, 10);
                            } while (choix != 0 && choix != 1 && choix != -1);
                            poids[j][i][k] = choix;
                        }
                    }
                }
            }

            int **seuils = malloc(nombrecouches * sizeof(int **));
            for (int j = 0; j < nombrecouches; ++j) // poids
            {
                seuils[j] = malloc(listenombre[j] * sizeof(int *));
                for (int i = 0; i < listenombre[j]; ++i)
                {
                    choix = -1;
                    reponse = 'a';
                    do
                    {
                        printf("\n");
                        printf("-------------------------------------------------\n");
                        printf("- seuil du neurone %d de la couche %d :         -\n", i + 1, j + 1);
                        printf("-------------------------------------------------\n");
                        scanf("%s", &reponse);
                        choix = strtol(&reponse, NULL, 10);
                    } while (choix <= 0);
                    seuils[i][j] = choix;
                }
            }

            // initalisation Entrer
            int Entry[nEntry];
            Choix_Entry(nEntry, Entry);

            Reseau R = CreateR(nombrecouches, listenombre, nEntry, poids, seuils); // initialise le Reseau
            int *sortie = OutR(R, Entry, nombrecouches);

            printf(" La sortie de votre expression est %d", sortie[0]);
            break;
        }
        default:
            printf("Erreur de saisie");
        }
    } while (choixmenu != 1 && choixmenu != 2 && choixmenu != 3 && choixmenu != 4 && choixmenu != 5);
}

int main()
{
    menu();
    return 0;
}
