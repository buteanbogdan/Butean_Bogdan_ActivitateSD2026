//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
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
//Laptop citireLaptopDinFisier(FILE* file) {
//    char buffer[100];
//    char sep[3] = ",\n";
//    Laptop l1;
//    l1.id = -1; 
//
//    if (fgets(buffer, 100, file)) {
//        char* aux;
//
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
//void afisareLaptop(Laptop l) {
//    printf("ID: %d | Nuclee: %d | Pret: %.2f | Model: %s | Producator: %s | Serie: %c\n",
//        l.id, l.nrNuclee, l.pret, l.model, l.producator, l.serie);
//}
//
//void afisareListaLaptopuri(Nod* cap) {
//    while (cap != NULL) {
//        afisareLaptop(cap->info);
//        cap = cap->next;
//    }
//}
//
//void adaugaLaptopInLista(Nod** cap, Laptop laptopNou) {
//    Nod* nou = (Nod*)malloc(sizeof(Nod));
//    nou->info = laptopNou;
//    nou->next = NULL;
//    if (*cap) {
//        Nod* p = *cap;
//        while (p->next) {
//            p = p->next;
//        }
//        p->next = nou;
//    }
//    else {
//        *cap = nou;
//    }
//}
//
//Nod* citireListaLaptopuriDinFisier(const char* numeFisier) {
//    Nod* cap = NULL;
//    FILE* f = fopen(numeFisier, "r");
//    if (f) {
//        while (!feof(f)) {
//            Laptop l = citireLaptopDinFisier(f);
//            if (l.id != -1) {
//                adaugaLaptopInLista(&cap, l);
//            }
//        }
//        fclose(f);
//    }
//    return cap;
//}
//
//void dezalocareLista(Nod** cap) {
//    while (*cap) {
//        Nod* p = *cap;
//        (*cap) = p->next;
//        free(p->info.model);
//        free(p->info.producator);
//        free(p);
//    }
//}
//
//float calculeazaPretMediu(Nod* cap) {
//    float suma = 0;
//    int contor = 0;
//    while (cap) {
//        suma += cap->info.pret;
//        contor++;
//        cap = cap->next;
//    }
//    return (contor > 0) ? (suma / contor) : 0;
//}
//
//float calculeazaPretTotalProducator(Nod* cap, const char* producator) {
//    float suma = 0;
//    while (cap) {
//        if (strcmp(cap->info.producator, producator) == 0) {
//            suma += cap->info.pret;
//        }
//        cap = cap->next;
//    }
//    return suma;
//}
//
//void stergeLaptopuriSerie(Nod** cap, char serieCautata) {
//    while ((*cap) && (*cap)->info.serie == serieCautata) {
//        Nod* aux = *cap;
//        *cap = aux->next;
//        free(aux->info.model);
//        free(aux->info.producator);
//        free(aux);
//    }
//    if (*cap) {
//        Nod* p = *cap;
//        while (p->next) {
//            if (p->next->info.serie == serieCautata) {
//                Nod* aux = p->next;
//                p->next = aux->next;
//                free(aux->info.model);
//                free(aux->info.producator);
//                free(aux);
//            }
//            else {
//                p = p->next;
//            }
//        }
//    }
//}
//
//int main() {
//
//    Nod* cap = citireListaLaptopuriDinFisier("laptopuri.txt");
//
//    printf("--- Lista Laptopuri ---\n");
//    afisareListaLaptopuri(cap);
//
//    printf("\nPretul mediu este: %.2f\n", calculeazaPretMediu(cap));
//
//    const char* brand = "Asus";
//    printf("Pretul total pentru %s este: %.2f\n", brand, calculeazaPretTotalProducator(cap, brand));
//
//    printf("\nStergere seria A...\n");
//    stergeLaptopuriSerie(&cap, 'A');
//    afisareListaLaptopuri(cap);
//
//    dezalocareLista(&cap);
//    return 0;
//}