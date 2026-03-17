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

void afisareLaptop(Laptop laptop) {
	printf("ID:%d\n", laptop.id);
	printf("Numar nuclee: %d\n", laptop.nrNuclee);
	printf("Pretul: %.2f \n", laptop.pret);
	printf("Model: %s\n", laptop.model);
	printf("Producator: %s\n", laptop.producator);
	printf("Serie: %c\n", laptop.serie);

}

void afisareVectorLaptop(Laptop* laptopuri, int nrLaptopuri) {
	for (int i = 0; i < nrLaptopuri; i++) {
		afisareLaptop(laptopuri[i]);
	}
}

void adaugaLaptopInVector(Laptop** laptopuri, int* nrLaptopuri, Laptop laptopNou) {
	Laptop* aux = (Laptop*)malloc(sizeof(Laptop) * ((*nrLaptopuri) + 1));
	for (int i = 0; i < *nrLaptopuri; i++) {
		aux[i] = (*laptopuri)[i];
	}
	aux[(*nrLaptopuri)] = laptopNou;
	free(*laptopuri);
	(*laptopuri) = aux;
	(*nrLaptopuri)++;
}

Laptop citireLaptopFisier(FILE* file) {
	Laptop l;
	char buffer[100];
	char sep[4] = ",;\n";
	fgets(buffer, 100, file);
	l.id = atoi(strtok(buffer, sep));
	l.nrNuclee = atoi(strtok(NULL, sep));
	l.pret = atof(strtok(NULL, sep));
	char* aux;
	aux = strtok(NULL, sep);
	l.model = (char*)malloc(strlen(aux) + 1);
	strcpy(l.model, aux);
	aux = strtok(NULL, sep);
	l.producator = (char*)malloc(strlen(aux) + 1);
	strcpy(l.producator, aux);
	l.serie = strtok(NULL, sep)[0];
	return l;
}

Laptop* citireVectorLaptopFisier(const char* numeFisier, int* nrLaptopCitite) {
	FILE* file = fopen(numeFisier, "r");
	Laptop* laptop = NULL;
	(*nrLaptopCitite) = 0;
	while (!feof(file)) {
		adaugaLaptopInVector(&laptop, nrLaptopCitite, citireLaptopFisier(file));
	}
	fclose(file);
	return laptop;
}

void dezalocareVectorLaptop(Laptop** vector, int* nrLaptopuri) {
	for (int i = 0; i < *nrLaptopuri; i++) {
		if ((*vector)[i].model != NULL) {
			free((*vector)[i].model);
		}
		if ((*vector)[i].producator != NULL) {
			free((*vector)[i].producator);
		}
	}
	free(*vector);
	(*vector) = NULL;
	(*nrLaptopuri) = 0;
	
}

int main() {
	int nrLaptopuri = 0;
	Laptop* laptopuri = citireVectorLaptopFisier("laptopuri.txt", &nrLaptopuri);
	afisareVectorLaptop(laptopuri, nrLaptopuri);

	return 0;
}