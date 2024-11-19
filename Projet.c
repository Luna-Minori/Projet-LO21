#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
    int Nentry;
    int *Entry;
    int *poids;
    int seuil;
} neurone;

typedef struct CN
{
    neurone N;
    struct CN *suivant;
    struct CN *precedent;
} Coucheneurone;

typedef Coucheneurone *ReseauNeuronne;

ReseauNeuronne CreateAND(ReseauNeuronne Neurone)
{
    ReseauNeuronne new = malloc(sizeof(Coucheneurone));
    new->N.poids = malloc(2 * sizeof(int));
    new->N.Entry = malloc(2 * sizeof(int));
    new->N.Nentry = 2;
    new->N.poids[0] = 1;
    new->N.poids[1] = 1;
    new->N.Entry[0] = 1;
    new->N.Entry[1] = 1;
    new->N.seuil = 2;
    new->suivant = NULL;
    if (Neurone == NULL)
    {
        new->precedent = NULL;
        return new;
    }

    ReseauNeuronne temp = Neurone;
    while (temp->suivant != NULL)
    {
        temp = temp->suivant;
    }
    new->precedent = temp;
    temp->suivant = new;
    return Neurone;
}

int OutputNeurone(ReseauNeuronne Neurone)
{
    int valeur = 0;
    if (Neurone == NULL)
    {
        printf("Erreur : Aucune entree fournie.\n");
        return -1;
    }

    for (int i = 0; i < Neurone->N.Nentry; i++)
    {
        valeur = valeur + Neurone->N.Entry[i] * Neurone->N.poids[i];
    }
    if (valeur >= Neurone->N.seuil)
    {
        return 1;
    }
    return 0;
}

int main()
{
    ReseauNeuronne AND = NULL;
    int l = 10;
    AND = CreateAND(AND);
    l = OutputNeurone(AND);
    printf("l : %d", l);
    return l;
}
