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
};
typedef struct Nod Nod;

struct Coada {
    Nod* fata;
    Nod* spate;
};
typedef struct Coada Coada;


Laptop citireLaptopDinFisier(FILE* file) {
    char buffer[100];
    char sep[] = ",\n";
    Laptop l1;
    l1.id = -1; 

    if (fgets(buffer, 100, file)) {
        char* aux = strtok(buffer, sep);
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



void pushStack(Nod** varf, Laptop laptopNou) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = laptopNou;
    nou->next = *varf;
    *varf = nou;
}

int emptyStack(Nod* varf) {
    return varf == NULL;
}

Laptop popStack(Nod** varf) {
    if (!emptyStack(*varf)) {
        Nod* temp = *varf;
        Laptop extras = temp->info;
        *varf = (*varf)->next;
        free(temp); 
        return extras;
    }
    Laptop eroare; eroare.id = -1; eroare.model = NULL;
    return eroare;
}

Nod* citireStackLaptopuriDinFisier(const char* numeFisier) {
    Nod* varf = NULL;
    FILE* f = fopen(numeFisier, "r");
    if (f) {
        while (!feof(f)) {
            Laptop l = citireLaptopDinFisier(f);
            if (l.id != -1) {
                pushStack(&varf, l);
            }
        }
        fclose(f);
    }
    return varf;
}

void dezalocareStivaDeLaptopuri(Nod** varf) {
    while (!emptyStack(*varf)) {
        Laptop extras = popStack(varf);
        free(extras.model);
        free(extras.producator);
    }
}

int size(Nod* varf) {
    int contor = 0;
    while (varf) {
        contor++;
        varf = varf->next;
    }
    return contor;
}



void enqueue(Coada* c, Laptop laptopNou) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = laptopNou;
    nou->next = NULL;

    if (c->spate == NULL) { 
        c->fata = nou;
        c->spate = nou;
    }
    else {
        c->spate->next = nou;
        c->spate = nou;
    }
}

int emptyQueue(Coada c) {
    return c.fata == NULL;
}

Laptop dequeue(Coada* c) {
    if (!emptyQueue(*c)) {
        Nod* temp = c->fata;
        Laptop extras = temp->info;
        c->fata = temp->next;

        if (c->fata == NULL) { 
            c->spate = NULL;
        }
        free(temp);
        return extras;
    }
    Laptop eroare; eroare.id = -1; eroare.model = NULL;
    return eroare;
}

Coada citireCoadaDeLaptopuriDinFisier(const char* numeFisier) {
    Coada c;
    c.fata = NULL;
    c.spate = NULL;
    FILE* f = fopen(numeFisier, "r");
    if (f) {
        while (!feof(f)) {
            Laptop l = citireLaptopDinFisier(f);
            if (l.id != -1) {
                enqueue(&c, l);
            }
        }
        fclose(f);
    }
    return c;
}

void dezalocareCoadaDeLaptopuri(Coada* c) {
    while (!emptyQueue(*c)) {
        Laptop extras = dequeue(c);
        free(extras.model);
        free(extras.producator);
    }
}



Laptop getLaptopByID(Nod** varfStiva, int idCautat) {
    Nod* stivaAuxiliara = NULL;
    Laptop gasit; gasit.id = -1; gasit.model = NULL;

 
    while (!emptyStack(*varfStiva)) {
        Laptop extrasa = popStack(varfStiva);
        if (extrasa.id == idCautat && gasit.id == -1) {
            gasit = extrasa; 
        }
        pushStack(&stivaAuxiliara, extrasa);
    }

    // Refacem stiva initiala
    while (!emptyStack(stivaAuxiliara)) {
        pushStack(varfStiva, popStack(&stivaAuxiliara));
    }

    return gasit;
}

float calculeazaPretTotalStiva(Nod** varfStiva) {
    Nod* stivaAuxiliara = NULL;
    float sumaTotala = 0;

    // Parcurgere cu descarcare
    while (!emptyStack(*varfStiva)) {
        Laptop extrasa = popStack(varfStiva);
        sumaTotala += extrasa.pret;
        pushStack(&stivaAuxiliara, extrasa);
    }

    // Incarcare la loc
    while (!emptyStack(stivaAuxiliara)) {
        pushStack(varfStiva, popStack(&stivaAuxiliara));
    }

    return sumaTotala;
}


Coada extrageLaptopuriDupaProducator(Coada* coadaOriginala, const char* producatorCautat) {
    // Coada in care vom pune doar laptopurile cautate
    Coada coadaFiltrata;
    coadaFiltrata.fata = NULL;
    coadaFiltrata.spate = NULL;

    // Coada in care vom pune temporar laptopurile care NU corespund
    Coada coadaAuxiliara;
    coadaAuxiliara.fata = NULL;
    coadaAuxiliara.spate = NULL;
    
    while (!emptyQueue(*coadaOriginala)) {
        Laptop extras = dequeue(coadaOriginala);
        if (strcmp(extras.producator, producatorCautat) == 0) {
            enqueue(&coadaFiltrata, extras); 
        }
        else {
            enqueue(&coadaAuxiliara, extras); 
        }
    }
   
    while (!emptyQueue(coadaAuxiliara)) {
        enqueue(coadaOriginala, dequeue(&coadaAuxiliara));
    }

    return coadaFiltrata;
}
int main() {


    // 1. TESTARE STIVA
    printf("--- TESTARE STIVA (LIFO) ---\n");
    Nod* stiva = citireStackLaptopuriDinFisier("laptopuri.txt");

    printf("Numar elemente in stiva: %d\n", size(stiva));
    printf("Pret total stiva: %.2f\n", calculeazaPretTotalStiva(&stiva));

    printf("\nCautam laptop ID 2 in stiva:\n");
    Laptop cautat = getLaptopByID(&stiva, 2);
    if (cautat.id != -1) afisareLaptop(cautat);

    printf("\nExtragem (Pop) elementele din stiva (vor iesi invers fata de fisier):\n");
    while (!emptyStack(stiva)) {
        Laptop extrasa = popStack(&stiva);
        afisareLaptop(extrasa);
        free(extrasa.model); free(extrasa.producator);
    }

    // 2. TESTARE COADA
    printf("\n--- TESTARE COADA (FIFO) ---\n");
    Coada coada = citireCoadaDeLaptopuriDinFisier("laptopuri.txt");

    const char* brandCautat = "Asus";
    printf("\nExtragem toate laptopurile '%s' intr-o coada separata...\n", brandCautat);

    // Apelam functia de filtrare pe coada plina!
    Coada coadaDoarAsus = extrageLaptopuriDupaProducator(&coada, brandCautat);

    printf("\n--- Coada Noua (Doar %s) ---\n", brandCautat);
    while (!emptyQueue(coadaDoarAsus)) {
        Laptop extrasa = dequeue(&coadaDoarAsus);
        afisareLaptop(extrasa);
        free(extrasa.model); free(extrasa.producator);
    }

    printf("\n--- Coada Originala (Ce a mai ramas) ---\n");
    while (!emptyQueue(coada)) {
        Laptop extrasa = dequeue(&coada);
        afisareLaptop(extrasa);
        free(extrasa.model); free(extrasa.producator);
    }

   
    return 0;
}