#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraLaptop {
    int id;
    int nrNuclee;
    float pret;
    char* model;
    char* producator;
    unsigned char serie;
};
typedef struct StructuraLaptop Laptop;

struct Nod {
    Laptop info;
    struct Nod* next;
    struct Nod* prev; 
};
typedef struct Nod Nod;


struct LDI {
    Nod* prim;
    Nod* ultim;
};
typedef struct LDI LDI;

Laptop citireLaptopDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    Laptop l1;
    l1.id = -1;

    if (fgets(buffer, 100, file)) {
        char* aux;
        aux = strtok(buffer, sep);
        if (aux) l1.id = atoi(aux);

        aux = strtok(NULL, sep);
        if (aux) l1.nrNuclee = atoi(aux);

        aux = strtok(NULL, sep);
        if (aux) l1.pret = (float)atof(aux);

        aux = strtok(NULL, sep);
        if (aux) {
            l1.model = (char*)malloc(strlen(aux) + 1);
            strcpy(l1.model, aux);
        }

        aux = strtok(NULL, sep);
        if (aux) {
            l1.producator = (char*)malloc(strlen(aux) + 1);
            strcpy(l1.producator, aux);
        }

        aux = strtok(NULL, sep);
        if (aux) l1.serie = aux[0];
    }
    return l1;
}

void afisareLaptop(Laptop l) {
    printf("ID: %d | Nuclee: %d | Pret: %.2f | Model: %s | Producator: %s | Serie: %c\n",
        l.id, l.nrNuclee, l.pret, l.model, l.producator, l.serie);
}


void inserareSfarsit(LDI* lista, Laptop l) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = l;
    nou->next = NULL;
    nou->prev = lista->ultim;

    if (lista->ultim) {
        lista->ultim->next = nou;
        lista->ultim = nou;
    }
    else {
        lista->prim = nou;
        lista->ultim = nou;
    }
}

void afisareDeLaInceput(LDI lista) {
    Nod* temp = lista.prim;
    while (temp) {
        afisareLaptop(temp->info);
        temp = temp->next;
    }
}

void afisareDeLaSfarsit(LDI lista) {
    Nod* temp = lista.ultim;
    while (temp) {
        afisareLaptop(temp->info);
        temp = temp->prev;
    }
}

void dezalocareLDI(LDI* lista) {
    Nod* temp = lista->prim;
    while (temp) {
        Nod* aux = temp;
        temp = temp->next;
        free(aux->info.model);
        free(aux->info.producator);
        free(aux);
    }
    lista->prim = NULL;
    lista->ultim = NULL;
}


void stergeLaptopuriSerie(LDI* lista, char serie) {
    Nod* temp = lista->prim;
    while (temp) {
        if (temp->info.serie == serie) {
            if (temp->prev) {
                temp->prev->next = temp->next;
            }
            else {
                lista->prim = temp->next;
            }

            if (temp->next) {
                temp->next->prev = temp->prev;
            }
            else {
                lista->ultim = temp->prev;
            }

            Nod* deSters = temp;
            temp = temp->next; 

            free(deSters->info.model);
            free(deSters->info.producator);
            free(deSters);
        }
        else {
            temp = temp->next;
        }
    }
}

int main() {
    LDI lista;
    lista.prim = NULL;
    lista.ultim = NULL;

    FILE* f = fopen("laptopuri.txt", "r");
    if (f) {
        while (!feof(f)) {
            Laptop l = citireLaptopDinFisier(f);
            if (l.id != -1) {
                inserareSfarsit(&lista, l);
            }
        }
        fclose(f);
    }

    printf("--- Afisare de la inceput (LDI) ---\n");
    afisareDeLaInceput(lista);

    printf("\n--- Afisare de la sfarsit (LDI) ---\n");
    afisareDeLaSfarsit(lista);

    printf("\nStergere laptopuri seria 'A'...\n");
    stergeLaptopuriSerie(&lista, 'A');
    afisareDeLaInceput(lista);

    dezalocareLDI(&lista);
    return 0;
}