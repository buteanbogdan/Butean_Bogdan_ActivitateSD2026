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

struct Heap {
    Laptop* vector;
    int dimensiune;
    int dimensiuneTotala;
};
typedef struct Heap Heap;


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
    printf("ID: %d | Model: %s | Pret: %.2f\n", l.id, l.model, l.pret);
}


void filtrareHeap(Heap heap, int pozitie) {
    int fiuStanga = 2 * pozitie + 1;
    int fiuDreapta = 2 * pozitie + 2;
    int maxIdx = pozitie;

    if (fiuStanga < heap.dimensiune && heap.vector[fiuStanga].pret > heap.vector[maxIdx].pret) {
        maxIdx = fiuStanga;
    }

    if (fiuDreapta < heap.dimensiune && heap.vector[fiuDreapta].pret > heap.vector[maxIdx].pret) {
        maxIdx = fiuDreapta;
    }

    if (maxIdx != pozitie) {
        Laptop aux = heap.vector[pozitie];
        heap.vector[pozitie] = heap.vector[maxIdx];
        heap.vector[maxIdx] = aux;
        filtrareHeap(heap, maxIdx);
    }
}

// Inserare in Heap
void inserareHeap(Heap* heap, Laptop l) {
    if (heap->dimensiune < heap->dimensiuneTotala) {
        heap->vector[heap->dimensiune] = l;
        int copil = heap->dimensiune;
        int parinte = (copil - 1) / 2;

        while (copil > 0 && heap->vector[copil].pret > heap->vector[parinte].pret) {
            Laptop aux = heap->vector[copil];
            heap->vector[copil] = heap->vector[parinte];
            heap->vector[parinte] = aux;

            copil = parinte;
            parinte = (copil - 1) / 2;
        }
        heap->dimensiune++;
    }
}

// Extragere element radacina (laptopul cel mai scump)
Laptop extragereMax(Heap* heap) {
    if (heap->dimensiune > 0) {
        Laptop rezultat = heap->vector[0];
        heap->vector[0] = heap->vector[heap->dimensiune - 1];
        heap->dimensiune--;
        filtrareHeap(*heap, 0);
        return rezultat;
    }
    Laptop l; l.id = -1; return l;
}

void dezalocareHeap(Heap* heap) {
    for (int i = 0; i < heap->dimensiune; i++) {
        free(heap->vector[i].model);
        free(heap->vector[i].producator);
    }
    free(heap->vector);
    heap->vector = NULL;
    heap->dimensiune = 0;
}

int main() {
    Heap heap;
    heap.dimensiune = 0;
    heap.dimensiuneTotala = 10;
    heap.vector = (Laptop*)malloc(sizeof(Laptop) * heap.dimensiuneTotala);

    FILE* f = fopen("laptopuri.txt", "r");
    if (f) {
        while (!feof(f)) {
            Laptop l = citireLaptopDinFisier(f);
            if (l.id != -1) {
                inserareHeap(&heap, l);
            }
        }
        fclose(f);
    }

    printf("--- Max-Heap Laptopuri (ordonat dupa pret) ---\n");
    for (int i = 0; i < heap.dimensiune; i++) {
        afisareLaptop(heap.vector[i]);
    }

    printf("\nExtragere laptop cel mai scump:\n");
    Laptop scump = extragereMax(&heap);
    afisareLaptop(scump);
   
    free(scump.model);
    free(scump.producator);

    printf("\nHeap dupa extragere:\n");
    for (int i = 0; i < heap.dimensiune; i++) {
        afisareLaptop(heap.vector[i]);
    }

    dezalocareHeap(&heap);
    return 0;
}