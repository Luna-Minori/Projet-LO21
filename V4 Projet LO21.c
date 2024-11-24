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

    for (int i = 0; i < n_Entry; i++)
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
    for (int i = 0; i < nombre_neurones; i++)
    {
        Neurone *new = InitNeurone(poids[i], seuil[i], n_Entry);
        if (i == 0)
        {
            couche->debutN = new;
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

    for (int i = 0; i < neurone->nombre_entrees; i++)
    {
        valeur = valeur + neurone->poids[i] * Entry[i];
    }

    if (valeur >= neurone->seuil)
    {
        return 1;
    }
    return 0;
}

int *OutCouche(Coucheneurone *couche, int *Entry)
{
    int *sorties = (int *)malloc(couche->nombre_neurones * sizeof(int));
    Neurone *tempN = NULL;

    for (int i = 0; i < couche->nombre_neurones; i++)
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

Reseau CreateR(int nombrecouches, int *Nneurone, int n_Entry, int **poids, int *seuil)
{
    Reseau R;
    Coucheneurone *precedente = NULL;

    for (int i = 0; i < nombrecouches; i++)
    {
        Coucheneurone *newcouche = InitCouche(Nneurone[i], n_Entry, poids, seuil);

        if (i == 0)
        {
            R.debutC = newcouche;
        }
        else
        {
            precedente->suivante = newcouche;
            newcouche->precedente = precedente;
        }

        precedente = newcouche;
        n_Entry = Nneurone[i];
    }

    R.finC = precedente;

    return R;
}

int *OutR(Reseau R, int *Entry, int nombrecouches)
{
    int *sorties = Entry;

    Coucheneurone *couche = R.debutC;
    for (int i = 0; i < nombrecouches; i++)
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
            int *poids[] = {poids1};
            int seuil[] = {Nentry};

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
            int *poids[] = {poids1};
            int seuil[] = {1};

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
            int *poids[] = {poids1};
            int seuil[] = {0};

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
            int poids1[] = {1, 0, 0};
            int poids2[] = {0, -1, 0};
            int poids3[] = {0, 0, 1};
            int poids4[] = {0, 0, -1};
            int poids5[] = {1, 1, 1, 0};
            int poids6[] = {1, 0, 0, 1};
            int poids7[] = {1, 1};
            int *poids[] = {poids1, poids2, poids3, poids4, poids5, poids6, poids7};
            int seuil[] = {1, 0, 1, 0, 3, 2, 1};

            Reseau R = CreateR(nombrecouches, listenombre, Nentry, poids, seuil); // initialise le Reseau
            int *sortie = OutR(R, Entry, nombrecouches);                          // Calcule le résultat

            printf("Sortie pour A=%d, B=%d, C=%d de l'expression logique : (A ET (non B) ET C) OU (A ET (non C)) est : %d\n", Entry[0], Entry[1], Entry[2], sortie[0]);
            break;
        }
        default:
            printf("Erreur de saisie");
        }
    } while (choixmenu != 1 && choixmenu != 2 && choixmenu != 3 && choixmenu != 4);
}

int main()
{
    menu();
    return 0;
}
