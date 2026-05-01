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
	printf("[ID: %d] Producator: %s | Model: %s | Nuclee: %d | Pret: %.2f | Serie: %c\n",
		laptop.id, laptop.producator, laptop.model, laptop.nrNuclee, laptop.pret, laptop.serie);
}


typedef struct NodSecundar NodSecundar;
typedef struct NodPrincipal NodPrincipal;

struct NodPrincipal {
	Laptop info;
	NodSecundar* vecini;
	struct NodPrincipal* next;
};

struct NodSecundar {
	NodPrincipal* nodInfo;
	NodSecundar* next;
};


void inserareListaPrincipala(NodPrincipal** cap, Laptop l) {
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nou->info = l;
	nou->next = NULL;
	nou->vecini = NULL;
	if (*cap) {
		NodPrincipal* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou;
	}
}

void inserareListaSecundara(NodSecundar** cap, NodPrincipal* nodInfo) {
	NodSecundar* nou = (NodSecundar*)malloc(sizeof(NodSecundar));
	nou->nodInfo = nodInfo;
	nou->next = *cap;
	*cap = nou; 
}

NodPrincipal* cautaNodDupaID(NodPrincipal* cap, int idCautat) {
	while (cap && cap->info.id != idCautat) {
		cap = cap->next;
	}
	return cap;
}

void inserareMuchie(NodPrincipal* graf, int idStart, int idStop) {
	NodPrincipal* nodStart = cautaNodDupaID(graf, idStart);
	NodPrincipal* nodStop = cautaNodDupaID(graf, idStop);
	if (nodStart && nodStop) {
		inserareListaSecundara(&(nodStart->vecini), nodStop);
		inserareListaSecundara(&(nodStop->vecini), nodStart);
	}
}


NodPrincipal* citireNoduriLaptopDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodPrincipal* graf = NULL;
	if (f) {
		while (!feof(f)) {
			Laptop l = citireLaptopDinFisier(f);
			if (l.id != -1) {
				inserareListaPrincipala(&graf, l);
			}
		}
		fclose(f);
	}
	return graf;
}

void citireMuchiiDinFisier(NodPrincipal* graf, const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			int idStart = 0;
			int idStop = 0;
			if (fscanf(f, "%d %d", &idStart, &idStop) == 2) {
				inserareMuchie(graf, idStart, idStop);
			}
		}
		fclose(f);
	}
}


void dezalocareListaSecundara(NodSecundar** cap) {
	while (*cap) {
		NodSecundar* temp = (*cap);
		(*cap) = (*cap)->next;
		free(temp);
	}
}

void dezalocareNoduriGraf(NodPrincipal** graf) {
	while (*graf) {
		NodPrincipal* temp = (*graf);
		(*graf) = (*graf)->next;
		dezalocareListaSecundara(&(temp->vecini));
		if (temp->info.model) {
			free(temp->info.model);
		}
		if (temp->info.producator) {
			free(temp->info.producator);
		}
		free(temp);
	}
}


typedef struct NodStiva NodStiva;
struct NodStiva {
	int id;
	NodStiva* next;
};

void push(NodStiva** cap, int id) {
	NodStiva* nou = (NodStiva*)malloc(sizeof(NodStiva));
	nou->id = id;
	nou->next = *cap;
	*cap = nou;
}

int pop(NodStiva** cap) {
	if (*cap) {
		NodStiva* temp = *cap;
		*cap = (*cap)->next;
		int id = temp->id;
		free(temp);
		return id;
	}
	return -1;
}

int calculeazaNrNoduriGraf(NodPrincipal* listaPrincipala) {
	int count = 0;
	while (listaPrincipala) {
		count++;
		listaPrincipala = listaPrincipala->next;
	}
	return count;
}

void afisareGrafInAdancime(NodPrincipal* graf, int idPlecare) {
	int nrNoduri = calculeazaNrNoduriGraf(graf);
	unsigned char* vizitate = (unsigned char*)malloc(sizeof(unsigned char) * nrNoduri);
	for (int i = 0; i < nrNoduri; i++) {
		vizitate[i] = 0;
	}

	NodStiva* stiva = NULL;
	vizitate[idPlecare - 1] = 1;
	push(&stiva, idPlecare);

	while (stiva) {
		int idNod = pop(&stiva);
		NodPrincipal* nodCurent = cautaNodDupaID(graf, idNod);
		afisareLaptop(nodCurent->info);

		NodSecundar* p = nodCurent->vecini;
		while (p) {
			
			if (vizitate[p->nodInfo->info.id - 1] == 0) {
				push(&stiva, p->nodInfo->info.id);
				vizitate[p->nodInfo->info.id - 1] = 1;
			}
			p = p->next;
		}
	}
	free(vizitate);
}


typedef struct NodCoada NodCoada;
struct NodCoada {
	int id;
	NodCoada* next;
};

void enqueue(NodCoada** cap, int id) {
	NodCoada* nou = (NodCoada*)malloc(sizeof(NodCoada)); 
	nou->id = id;
	nou->next = NULL;
	if (*cap) {
		NodCoada* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou;
	}
}

int dequeue(NodCoada** cap) {
	if (*cap) {
		NodCoada* temp = *cap;
		*cap = (*cap)->next;
		int id = temp->id;
		free(temp);
		return id;
	}
	return -1;
}

void afisareGrafInLatime(NodPrincipal* graf, int idPlecare) {
	int nrNoduri = calculeazaNrNoduriGraf(graf);
	unsigned char* vizitate = (unsigned char*)malloc(sizeof(unsigned char) * nrNoduri);
	for (int i = 0; i < nrNoduri; i++) {
		vizitate[i] = 0;
	}

	NodCoada* coada = NULL;
	vizitate[idPlecare - 1] = 1;
	enqueue(&coada, idPlecare);

	while (coada) {
		int idNod = dequeue(&coada);
		NodPrincipal* nodCurent = cautaNodDupaID(graf, idNod);
		afisareLaptop(nodCurent->info);

		NodSecundar* p = nodCurent->vecini;
		while (p) {
			if (vizitate[p->nodInfo->info.id - 1] == 0) {
				enqueue(&coada, p->nodInfo->info.id);
				vizitate[p->nodInfo->info.id - 1] = 1;
			}
			p = p->next;
		}
	}
	free(vizitate);
}

int numaraVeciniDupaSerie(NodPrincipal* graf, int idCautat, unsigned char serieCautata) {
	
	NodPrincipal* nodCurent = cautaNodDupaID(graf, idCautat);

	if (nodCurent == NULL) {
		printf("Laptopul cu ID %d nu a fost gasit in graf.\n", idCautat);
		return 0;
	}

	int contor = 0;
	
	NodSecundar* p = nodCurent->vecini;

	while (p != NULL) {
		if (p->nodInfo->info.serie == serieCautata) {
			contor++;
		}
		p = p->next;
	}

	return contor;
}

int main() {

	NodPrincipal* graf = citireNoduriLaptopDinFisier("laptopuri.txt");
	citireMuchiiDinFisier(graf, "muchii.txt");

	printf("=== Parcurgere in LATIME (BFS) incepand cu ID 1 ===\n");
	afisareGrafInLatime(graf, 1);

	printf("\n=== Parcurgere in ADANCIME (DFS) incepand cu ID 1 ===\n");
	afisareGrafInAdancime(graf, 1);

	int idTest = 1;
	unsigned char serieTest = 'A';
	int nr = numaraVeciniDupaSerie(graf, idTest, serieTest);

	printf("\nLaptopul %d are %d vecini din seria %c.\n", idTest, nr, serieTest);

	dezalocareNoduriGraf(&graf);

	return 0;
}