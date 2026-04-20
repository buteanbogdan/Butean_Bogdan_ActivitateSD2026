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


struct NodSecundar; 


struct NodPrincipal {
	Laptop info;
	struct NodSecundar* vecini; 
	struct NodPrincipal* next;  
};
typedef struct NodPrincipal NodPrincipal;


struct NodSecundar {
	NodPrincipal* nodInfo;     
	struct NodSecundar* next;  
};
typedef struct NodSecundar NodSecundar;


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

void afisareLaptop(Laptop laptop) {
	printf("[ID: %d] %s %s - %.2f RON\n",
		laptop.id, laptop.producator, laptop.model, laptop.pret);
}


void inserarePrincipala(NodPrincipal** graf, Laptop laptopNou) {
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nou->info = laptopNou;
	nou->vecini = NULL;
	nou->next = NULL;

	if (*graf == NULL) {
		*graf = nou;
	}
	else {
		NodPrincipal* temp = *graf;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}


void inserareSecundara(NodSecundar** capSecundar, NodPrincipal* nodVecin) {
	NodSecundar* nou = (NodSecundar*)malloc(sizeof(NodSecundar));
	nou->nodInfo = nodVecin;
	nou->next = NULL;

	if (*capSecundar == NULL) {
		*capSecundar = nou;
	}
	else {
		NodSecundar* temp = *capSecundar;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}


NodPrincipal* cautaNodDupaID(NodPrincipal* graf, int id) {
	while (graf != NULL) {
		if (graf->info.id == id) {
			return graf;
		}
		graf = graf->next;
	}
	return NULL;
}


void inserareMuchie(NodPrincipal* graf, int idStart, int idStop) {
	NodPrincipal* nodStart = cautaNodDupaID(graf, idStart);
	NodPrincipal* nodStop = cautaNodDupaID(graf, idStop);

	
	if (nodStart != NULL && nodStop != NULL) {
		inserareSecundara(&(nodStart->vecini), nodStop);
		inserareSecundara(&(nodStop->vecini), nodStart); 
	}
	else {
		printf("Eroare: Unul dintre ID-urile %d sau %d nu exista in graf!\n", idStart, idStop);
	}
}


NodPrincipal* citireNoduriLaptopuriDinFisier(const char* numeFisier) {
	NodPrincipal* graf = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			Laptop l = citireLaptopDinFisier(f);
			if (l.id != -1) {
				inserarePrincipala(&graf, l);
			}
		}
		fclose(f);
	}
	return graf;
}


void citireMuchiiDinFisier(NodPrincipal* graf, const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		char buffer[50];
		while (fgets(buffer, 50, f)) {
			int idStart = atoi(strtok(buffer, ","));
			int idStop = atoi(strtok(NULL, ",\n"));
			inserareMuchie(graf, idStart, idStop);
		}
		fclose(f);
	}
}


void afisareGraf(NodPrincipal* graf) {
	NodPrincipal* tempGraf = graf;
	while (tempGraf != NULL) {
		printf("Laptopul principal: ");
		afisareLaptop(tempGraf->info);

		printf("  Are conexiune cu:\n");
		NodSecundar* vecini = tempGraf->vecini;
		if (vecini == NULL) {
			printf("    (niciun vecin)\n");
		}
		while (vecini != NULL) {
			printf("    -> ");
			afisareLaptop(vecini->nodInfo->info);
			vecini = vecini->next;
		}
		printf("\n");

		tempGraf = tempGraf->next;
	}
}


void dezalocareNoduriGraf(NodPrincipal** graf) {
	while (*graf != NULL) {
		NodPrincipal* nodDeSters = *graf;
		*graf = (*graf)->next;

		
		NodSecundar* tempVecin = nodDeSters->vecini;
		while (tempVecin != NULL) {
			NodSecundar* vecinDeSters = tempVecin;
			tempVecin = tempVecin->next;
			free(vecinDeSters);
		}

		free(nodDeSters->info.model);
		free(nodDeSters->info.producator);

	
		free(nodDeSters);
	}
}

int main() {
	
	NodPrincipal* graf = citireNoduriLaptopuriDinFisier("laptopuri.txt");


	citireMuchiiDinFisier(graf, "muchii.txt");

	
	printf("--- REPREZENTARE GRAF (LISTA DE LISTE) ---\n\n");
	afisareGraf(graf);

	dezalocareNoduriGraf(&graf);

	return 0;
}