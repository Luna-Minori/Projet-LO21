#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct neur
{
    int Nentry;
    int *Entry;
    int *poids;
    float seuil;
    int Result;
    int treated;
    struct neur *Nsuivant;
    struct neur *Nprecedent;
} neurone;

typedef struct CoucheN
{
    struct CoucheN *Csuivant;
    struct CoucheN *Cprecedent;
    neurone *neurone;
} Coucheneurone;

typedef Coucheneurone *ReseauNeurone;

ReseauNeurone ajout_neurone(ReseauNeurone R, neurone *new)
{
    if (R == NULL)
    {
        R = malloc(sizeof(Coucheneurone));
        R->neurone = new;
        R->neurone->Nsuivant = NULL;
        R->neurone->Nprecedent = NULL;
        R->Csuivant = NULL;
        R->Cprecedent = NULL;
        return R;
    }
    neurone *temp = R->neurone;
    while (temp->Nsuivant != NULL)
    {
        temp = temp->Nsuivant;
    }
    temp->Nsuivant = new;
    new->Nprecedent = temp;
    return R;
}

neurone *Createneurone(int *Entry, int size, float seuil)
{
    neurone *new = malloc(sizeof(neurone));
    new->poids = malloc(size * sizeof(int));
    new->Entry = malloc(size * sizeof(int));
    new->Nentry = size;
    new->Result = -1;
    new->treated = 0;

    for (int i = 0; i < size; i++)
    {
        new->Entry[i] = Entry[i];
        new->poids[i] = 1;
    }
    new->seuil = seuil;

    return new;
}

int OutputNeurone(neurone *Neurone)
{
    int valeur = 0;
    if (Neurone == NULL)
    {
        printf("Erreur : Aucune entree fournie.\n");
    }
    for (int i = 0; i < Neurone->Nentry; i++)
    {
        valeur = valeur + Neurone->Entry[i] * Neurone->poids[i];
    }
    if (valeur >= Neurone->seuil)
    {
        Neurone->Result = 1;
    }
    else
    {
        Neurone->Result = 0;
    }
}

int Nombreneurone(ReseauNeurone R)
{
    printf("1");
    neurone *temp = R->neurone;
    int i = 0;
    printf("2");
    if (R->Csuivant == NULL)
    {
        printf("3");
        if (temp == NULL)
        {
            return 0;
        }
        printf("5");
        if (temp->Nsuivant == NULL)
        {
            return i + 1;
        }
        printf("6");
        while (temp->Nsuivant != NULL)
        {
            temp = temp->Nsuivant;
            i = i++;
            printf("g");
        }
        printf("4");
    }
    else
    {
        printf("al");
        Coucheneurone *Couche = R;
        while (Couche->Csuivant != NULL)
        {
            temp = Couche->neurone;
            while (Couche->neurone->Nsuivant != NULL)
            {
                temp = temp->Nsuivant;
                i++;
            }
        }
    }
    return i;
}

int *Tab_Tab(int *Tab1, int *Tab2)
{
    int *Result;
    for (int i = 0; i < sizeof(Tab1); i++)
    {
        Result[i] = Tab1[i];
    }
    for (int i = 0; i < sizeof(Tab2); i++)
    {
        Result[i] = Tab2[i];
    }
    return Result;
}

int *Recupneurone(neurone *neurone, int *Result)
{
    if (neurone->Nsuivant == NULL)
    {
        neurone->treated = 1;
        return Tab_Tab(Result, &neurone->Result);
    }
    int i = 0;
    while (neurone->Nsuivant != NULL)
    {
        if (neurone->treated == 0)
        {
            Result[i] = neurone->Result;
            printf("Result %d : $d", i, Result[i]);
            neurone->treated = 1;
        }
        neurone = neurone->Nsuivant;
        i++;
    }
    return Result;
}

int *Recupsortie(ReseauNeurone R)
{
    printf("aa");
    int *Result = malloc(sizeof(int) * Nombreneurone(R));
    printf("ba");
    int *Resulttemp;
    printf("aa");
    if (R->Csuivant == NULL)
    {
        return Recupneurone(R->neurone, Result);
    }
    Coucheneurone *temp = R;
    while (temp->Csuivant != NULL)
    {
        Resulttemp = Recupneurone(temp->neurone, Result);
        Result = Tab_Tab(Result, Resulttemp);
        temp = temp->Csuivant;
    }
    return Result;
}

/*
int *Entrer()
{
    char reponse;
    char reponse2;

    printf("cb voulez vous de couche ? (1-5)");
    do
    {
        printf("cb voulez vous de couche ? (1-5)");
        scanf(&reponse);
    } while (reponse != 1 || reponse != 2 || reponse != 3 || reponse != 4 reponse != 5);

    int Output[strint(reponse) + 1];
    Output[0] = strint(reponse);
    for (int i = 0; i < reponse; i++)
    {
        do
        {
            printf("cb voulez vous de neurone dans la couche %d ? (1-3)", i + 1);
            scanf(&reponse2);
        } while (reponse2 != 1 || reponse2 != 2 || reponse2 != 3);
        Output[i + 1] = reponse2;
    }
    return Output;
}

ReseauNeurone CreateReseauneurone(int *Entrer){

}
*/

int main()
{
    // Entrer();
    ReseauNeurone R = NULL;
    // crée deux neurone AND premiere couche
    int EntryAND[2];
    int EntryOR[2];
    int EntryNOT;
    float seuil = 2;
    int *Result;
    EntryAND[0] = 1;
    EntryAND[1] = 1;
    EntryOR[0] = 1;
    EntryOR[1] = 1;
    EntryNOT = 0;

    R = ajout_neurone(R, Createneurone(EntryAND, 2, seuil));
    R = ajout_neurone(R, Createneurone(EntryAND, 2, seuil));
    OutputNeurone(R->neurone);
    OutputNeurone(R->neurone->Nsuivant);
    printf("marche");
    Result = Recupsortie(R);
    printf(" 1 :  %d , 2 : %d", Result[0], Result[1]);
    /*
    // Crée 1 neurone OR deuxime couche
    seuil = 1;
    ajout_neurone(R, Createneurone(Result, seuil));
    OutputNeurone(R->neurone);
    Result = Recupsortie(R);
    // Crée 1 neurone NOT
    seuil = 0.5;
    ajout_neurone(R, Createneurone(Result, seuil));
    int l = OutputNeurone(R->neurone->Nsuivant);
    printf("l : %d", l);
    */
    return 0;
}
