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
}

Laptop* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaFisier()
	//numarul de masini este determinat prin numarul de citiri din fisier
	//ATENTIE - la final inchidem fisierul/stream-ul
}

void dezalocareVectorMasini(Laptop** vector, int* nrMasini) {
	//este dezalocat intreg vectorul de masini
}

int main() {


	return 0;
}