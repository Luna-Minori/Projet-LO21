#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct element {
    int valeur;
    struct element *suivant;
    struct element *precedent;
} ListeChain;

typedef ListeChain* ListeC;

ListeC AjouterQ(ListeC Liste, int n) {
    ListeChain *new = malloc(sizeof(ListeChain));
    new->valeur = n;
    new->suivant = NULL;

    if (Liste == NULL) {
        new->precedent = NULL;
        return new;
    }

    ListeC temp = Liste;
    while (temp->suivant != NULL) {
        temp = temp->suivant;
    }
    new->precedent = temp;
    temp->suivant = new;
    return Liste;
}


ListeC init(ListeC Liste) {
    srand(time(NULL));
    Liste = AjouterQ(Liste, rand() % 10 + 1);

    for (int i = 0; i < 10; i++) {
        Liste = AjouterQ(Liste, rand() % 10 + 1);
    }
    return Liste;
}

int neuroneAND(ListeC Entry, ListeC poids, int seuil) {
    int valeur = 0;
    if (Entry == NULL) {
        printf("Erreur : Aucune entree fournie.\n");
        return -1;
    }

    while (Entry != NULL && poids != NULL) {
        valeur += Entry->valeur * poids->valeur;
        printf("valeur = %d\n", valeur);
        Entry = Entry->suivant;
        poids = poids->suivant;
    }
    if (valeur > seuil) {
        return 1;
    }
    return 0;
}

int neuroneOR(ListeC Entry, ListeC poids, int seuil) {
    int valeur = 0;

    if (Entry == NULL) {
        printf("Erreur : Aucune entree fournie.\n");
        return -1;
    }

    while (Entry != NULL && poids != NULL) {
        valeur += Entry->valeur * poids->valeur;
        printf("valeur = %d\n", valeur);
        Entry = Entry->suivant;
        poids = poids->suivant;
    }
    if (valeur >= seuil) {
        return 1;
    }
    return 0;
}

int neuroneNOT(int Entry, int seuil) {

    if (Entry < seuil) {
        return 1;
    }
    return 0;
}


int main() {
    printf("aaa\n");

    ListeC Entry = NULL;
    ListeC poids = NULL;

    Entry = init(Entry);
    poids = init(poids);

    printf("Liste Entry : ");
    ListeC temp = Entry;
    while (temp != NULL) {
        printf("%d ", temp->valeur);
        temp = temp->suivant;
    }
    printf("\n");

    printf("Liste poids : ");
    temp = poids;
    while (temp != NULL) {
        printf("%d ", temp->valeur);
        temp = temp->suivant;
    }
    printf("\n");

    int seuil = 10;
    int resultAND = neuroneAND(Entry, poids, seuil);
    printf("Resultat du neurone AND: %d\n", resultAND);
    int resultOR = neuroneOR(Entry, poids, seuil);
    printf("Resultat du neurone OR: %d\n", resultOR);
    int resultNOT = neuroneNOT(resultAND, resultOR);
    printf("Résultat du neurone NOT: %d\n", resultNOT);
    return 0;
}
