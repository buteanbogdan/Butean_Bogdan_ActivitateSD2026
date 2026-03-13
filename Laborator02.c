#include<stdio.h>
#include<malloc.h>


struct Laptop {
	int id;
	int RAM;
	char* producator;
	float pret;
	char serie;
};

struct Laptop initializare(int id, int
	ram, const char* producator, float pret, char serie) {
	struct Laptop l;
	l.id = id;
	l.RAM = ram;
	l.producator = (char*)malloc(sizeof(char) * (strlen(producator) + 1));
	strcpy_s(l.producator, strlen(producator) + 1, producator);
	l.pret = pret;
	l.serie = serie;
	return l;
}

void afisare(struct Laptop l) {
	if (l.producator != NULL) {
		printf("%d. Laptopul %s seria %c are %d GB RAM si costa %5.2f RON.\n", l.id, l.producator, l.serie, l.RAM, l.pret);
	}
	else {
		printf("%d. Laptopul din seria %c are %d GB RAM si costa %5.2f RON.\n", l.id, l.serie, l.RAM, l.pret);
	}
}

void afisareVector(struct Laptop* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
	}
}

struct Laptop* copiazaPrimeleNElemente(struct Laptop* vector, int nrElemente, int nrElementeCopiate) {
	struct Laptop* vectorNou = NULL;
	vectorNou = (struct Laptop*)malloc(sizeof(struct Laptop) * nrElementeCopiate);
	for (int i = 0; i < nrElementeCopiate; i++) {
		vectorNou[i] = vector[i];
		vectorNou[i].producator = (char*)malloc(strlen(vector[i].producator) + 1);
		strcpy_s(vectorNou[i].producator, strlen(vector[i].producator) + 1, vector[i].producator);
	}

	return vectorNou;
}

void dezalocare(struct Laptop** vector, int* nrElemente) {
	for (int i = 0; i < (*nrElemente); i++) {
		if ((*vector)[i].producator != NULL){
		free((*vector)[i].producator);
		}
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaLaptopuriScumpe(struct Laptop* vector, char nrElemente, float pretMinim, struct Laptop** vectorNou, int* dimensiune) {
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret >= pretMinim) {
			(*dimensiune)++;
		}
	}
	if ((*vectorNou) != NULL){
		free(*vectorNou);
	}
	*vectorNou = (struct Laptop*)malloc(sizeof(struct Laptop) * (*dimensiune));
	int k = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret >= pretMinim) {
			(*vectorNou)[k] = vector[i];
			(*vectorNou)[k].producator = (char*)malloc(strlen(vector[i].producator) + 1);
			strcpy_s((*vectorNou)[k].producator, strlen(vector[i].producator) + 1, vector[i].producator);
			k++;
		}
	}
}

struct Laptop getPrimulLaptopByProducator(struct Laptop* vector, int nrElemente, const char* producator) {
	struct Laptop l;
	for (int i = 0; i < nrElemente; i++) {
		if (strcmp(vector[i].producator, producator) == 0) {
			l = vector[i];
			l.producator = (char*)malloc(strlen(vector[i].producator) + 1);
			strcpy_s(l.producator, strlen(vector[i].producator) + 1, vector[i].producator);
			return l;
		}
	}
	l.id = 1;

	return l;
}



int main() {

	struct Laptop* laptopuri = NULL;
	int nrLaptop = 3;
	laptopuri = (struct Laptop*)malloc(sizeof(struct Laptop) * nrLaptop);
	laptopuri[0] = initializare(1, 256, "Dell", 2000, 'E');
	laptopuri[1] = initializare(2, 512, "Lenovo", 3000, 'I');
	laptopuri[2] = initializare(1, 256, "Asus", 2500, 'G');
	afisareVector(laptopuri, nrLaptop);

	struct Laptop* primeleLaptopuri = NULL;
	int nrPrimeleLaptopuri = 2;
	primeleLaptopuri = copiazaPrimeleNElemente(laptopuri, nrLaptop, nrPrimeleLaptopuri);
	printf("\nPrimele laptopuri:\n");
	afisareVector(primeleLaptopuri, nrPrimeleLaptopuri);
	dezalocare(&primeleLaptopuri, &nrPrimeleLaptopuri);
	afisareVector(primeleLaptopuri, nrPrimeleLaptopuri);

	//copiaza laptopuri scumpe
	struct Laptop* laptopuriScumpe = NULL;
	int nrLaptopuriScumpe = 0;
	copiazaLaptopuriScumpe(laptopuri, nrLaptop, 2500, &laptopuriScumpe, &nrLaptopuriScumpe);
	printf("\nLaptopuri scumpe: \n");
	afisareVector(laptopuriScumpe, nrLaptopuriScumpe);
	dezalocare(&laptopuriScumpe, &nrLaptopuriScumpe);

	struct Laptop laptop = getPrimulLaptopByProducator(laptopuri, nrLaptop, "Lenovo");
	printf("\nLaptop by producator \n");
	afisare(laptop);
	if (laptop.producator != NULL) {
		free(laptop.producator);
		laptop.producator = NULL;
	}
	dezalocare(&laptopuri, &nrLaptop);
	return 0;
}