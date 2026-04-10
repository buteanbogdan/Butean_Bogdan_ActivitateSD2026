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


Nod* inserareABC(Nod* radacina, Laptop l) {
	if (radacina) {
		if (l.id < radacina->info.id) {
			radacina->st = inserareABC(radacina->st, l);
		}
		else if (l.id > radacina->info.id) {
			radacina->dr = inserareABC(radacina->dr, l);
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
	printf("ID: %d | Model: %s | Producator: %s | Pret: %.2f\n",
		l.id, l.model, l.producator, l.pret);
}


void afisareInordine(Nod* radacina) {
	if (radacina) {
		afisareInordine(radacina->st);
		afisareLaptop(radacina->info);
		afisareInordine(radacina->dr);
	}
}


Nod* cautareLaptop(Nod* radacina, int idCautat) {
	if (radacina) {
		if (idCautat == radacina->info.id) {
			return radacina;
		}
		else if (idCautat < radacina->info.id) {
			return cautareLaptop(radacina->st, idCautat);
		}
		else {
			return cautareLaptop(radacina->dr, idCautat);
		}
	}
	return NULL;
}


void dezalocareABC(Nod** radacina) {
	if (*radacina) {
		dezalocareABC(&((*radacina)->st));
		dezalocareABC(&((*radacina)->dr));
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
				radacina = inserareABC(radacina, l);
			}
		}
		fclose(f);
	}

	printf("--- Laptopuri in ABC (Sortate dupa ID) ---\n");
	afisareInordine(radacina);

	int idCautat = 2;
	printf("\nCautam laptopul cu ID %d:\n", idCautat);
	Nod* gasit = cautareLaptop(radacina, idCautat);
	if (gasit) {
		afisareLaptop(gasit->info);
	}
	else {
		printf("Laptopul nu a fost gasit.\n");
	}

	dezalocareABC(&radacina);
	return 0;
}