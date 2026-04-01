//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//struct StructuraLaptop {
//    int id;
//    int nrNuclee;
//    float pret;
//    char* model;
//    char* producator;
//    unsigned char serie;
//};
//typedef struct StructuraLaptop Laptop;
//
//struct Nod {
//    Laptop info;
//    struct Nod* next;
//};
//typedef struct Nod Nod;
//
//struct HashTable {
//    int dimensiune;
//    Nod** vector;
//};
//typedef struct HashTable HashTable;
//
//
//HashTable initializareHashTable(int dimensiune) {
//    HashTable ht;
//    ht.dimensiune = dimensiune;
//    ht.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);
//    for (int i = 0; i < dimensiune; i++) {
//        ht.vector[i] = NULL;
//    }
//    return ht;
//}
//
//
//int functieHash(int id, int dimensiune) {
//    return id % dimensiune;
//}
//
//
//Laptop citireLaptopDinFisier(FILE* file) {
//    char buffer[100];
//    char sep[3] = ",\n";
//    Laptop l1;
//    l1.id = -1;
//
//    if (fgets(buffer, 100, file)) {
//        char* aux;
//        aux = strtok(buffer, sep);
//        if (aux) l1.id = atoi(aux);
//
//        aux = strtok(NULL, sep);
//        if (aux) l1.nrNuclee = atoi(aux);
//
//        aux = strtok(NULL, sep);
//        if (aux) l1.pret = (float)atof(aux);
//
//        aux = strtok(NULL, sep);
//        if (aux) {
//            l1.model = (char*)malloc(strlen(aux) + 1);
//            strcpy(l1.model, aux);
//        }
//
//        aux = strtok(NULL, sep);
//        if (aux) {
//            l1.producator = (char*)malloc(strlen(aux) + 1);
//            strcpy(l1.producator, aux);
//        }
//
//        aux = strtok(NULL, sep);
//        if (aux) l1.serie = aux[0];
//    }
//    return l1;
//}
//
//
//void inserareHashTable(HashTable ht, Laptop l) {
//    if (ht.dimensiune > 0) {
//        int pozitie = functieHash(l.id, ht.dimensiune);
//
//        Nod* nou = (Nod*)malloc(sizeof(Nod));
//        nou->info = l;
//        nou->next = ht.vector[pozitie]; 
//        ht.vector[pozitie] = nou;
//    }
//}
//
//void afisareLaptop(Laptop l) {
//    printf("  ID: %d | Model: %s | Producator: %s | Pret: %.2f\n",
//        l.id, l.model, l.producator, l.pret);
//}
//
//void afisareHashTable(HashTable ht) {
//    for (int i = 0; i < ht.dimensiune; i++) {
//        printf("Pozitia %d:\n", i);
//        Nod* temp = ht.vector[i];
//        while (temp) {
//            afisareLaptop(temp->info);
//            temp = temp->next;
//        }
//    }
//}
//
//
//Laptop cautareLaptopInHashTable(HashTable ht, int idCautat) {
//    int pozitie = functieHash(idCautat, ht.dimensiune);
//    Nod* temp = ht.vector[pozitie];
//    while (temp) {
//        if (temp->info.id == idCautat) {
//            return temp->info;
//        }
//        temp = temp->next;
//    }
//    Laptop l; l.id = -1; l.model = NULL; l.producator = NULL;
//    return l;
//}
//
//void dezalocareHashTable(HashTable* ht) {
//    for (int i = 0; i < ht->dimensiune; i++) {
//        Nod* temp = ht->vector[i];
//        while (temp) {
//            Nod* aux = temp;
//            temp = temp->next;
//            free(aux->info.model);
//            free(aux->info.producator);
//            free(aux);
//        }
//    }
//    free(ht->vector);
//    ht->vector = NULL;
//    ht->dimensiune = 0;
//}
//
//int main() {
//    HashTable ht = initializareHashTable(5); 
//
//    FILE* f = fopen("laptopuri.txt", "r");
//    if (f) {
//        while (!feof(f)) {
//            Laptop l = citireLaptopDinFisier(f);
//            if (l.id != -1) {
//                inserareHashTable(ht, l);
//            }
//        }
//        fclose(f);
//    }
//
//    printf("--- Continut HashTable ---\n");
//    afisareHashTable(ht);
//
//    printf("\nCautare laptop cu ID 2:\n");
//    Laptop gasit = cautareLaptopInHashTable(ht, 2);
//    if (gasit.id != -1) {
//        afisareLaptop(gasit);
//    }
//    else {
//        printf("Laptopul nu a fost gasit.\n");
//    }
//
//    dezalocareHashTable(&ht);
//    return 0;
//}