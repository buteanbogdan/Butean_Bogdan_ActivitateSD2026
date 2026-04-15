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
	struct Nod* st;
	struct Nod* dr;
};
typedef struct Nod Nod;


int maxim_val(int a, int b) {
	return (a > b) ? a : b;
}


int inaltime(Nod* radacina) {
	if (radacina) {
		return 1 + maxim_val(inaltime(radacina->st), inaltime(radacina->dr));
	}
	else {
		return 0;
	}
}


int gradEchilibru(Nod* radacina) {
	if (radacina) {
		return inaltime(radacina->st) - inaltime(radacina->dr);
	}
	else {
		return 0;
	}
}


Nod* rotireDreapta(Nod* radacina) {
	Nod* nodSt = radacina->st;
	radacina->st = nodSt->dr;
	nodSt->dr = radacina;
	return nodSt;
}

Nod* rotireStanga(Nod* radacina) {
	Nod* nodDr = radacina->dr;
	radacina->dr = nodDr->st;
	nodDr->st = radacina;
	return nodDr;
}


Nod* inserareAVL(Nod* radacina, Laptop l) {
	if (radacina) {
		if (l.id < radacina->info.id) {
			radacina->st = inserareAVL(radacina->st, l);
		}
		else if (l.id > radacina->info.id) {
			radacina->dr = inserareAVL(radacina->dr, l);
		}
		else {
			return radacina; 
		}

		
		int grad = gradEchilibru(radacina);

		
		if (grad == 2 && gradEchilibru(radacina->st) == 1) {
			return rotireDreapta(radacina);
		}
		
		if (grad == -2 && gradEchilibru(radacina->dr) == -1) {
			return rotireStanga(radacina);
		}
		
		if (grad == 2 && gradEchilibru(radacina->st) == -1) {
			radacina->st = rotireStanga(radacina->st);
			return rotireDreapta(radacina);
		}
		
		if (grad == -2 && gradEchilibru(radacina->dr) == 1) {
			radacina->dr = rotireDreapta(radacina->dr);
			return rotireStanga(radacina);
		}

		return radacina;
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = l;
		nou->st = NULL;
		nou->dr = NULL;
		return nou;
	}
}

Laptop citireLaptopDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
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

void afisareInordine(Nod* radacina) {
	if (radacina) {
		afisareInordine(radacina->st);
		printf("ID: %d | Model: %s | Producator: %s | GE: %d\n",
			radacina->info.id, radacina->info.model, radacina->info.producator, gradEchilibru(radacina));
		afisareInordine(radacina->dr);
	}
}

void dezalocareAVL(Nod** radacina) {
	if (*radacina) {
		dezalocareAVL(&((*radacina)->st));
		dezalocareAVL(&((*radacina)->dr));
		free((*radacina)->info.model);
		free((*radacina)->info.producator);
		free(*radacina);
		*radacina = NULL;
	}
}

int main() {
	Nod* radacina = NULL;
	FILE* f = fopen("laptopuri.txt", "r");
	if (f) {
		while (!feof(f)) {
			Laptop l = citireLaptopDinFisier(f);
			if (l.id != -1) {
				radacina = inserareAVL(radacina, l);
			}
		}
		fclose(f);
	}

	printf("--- AVL Laptopuri (Auto-echilibrat) ---\n");
	afisareInordine(radacina);

	printf("\nInaltime arbore: %d\n", inaltime(radacina));

	dezalocareAVL(&radacina);
	return 0;
}