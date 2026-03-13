#include<stdio.h>
#include<stdlib.h>

struct Laptop {
	int id;
	int RAM;
	char* producator;
	float pret;
	char serie;
};

struct Laptop initializare(int id, int ram, const char* producator, float pret, char serie) {
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

void modificaPret(struct Laptop* l, float noulPret) {
	if (noulPret > 0) {
		l->pret = noulPret;
	}
}

void dezalocare(struct Laptop* l) {
	if (l->producator != NULL) {
		free(l->producator);
		l->producator = NULL;
	}
}

int main() {
	struct Laptop l;
	l = initializare(1, 256, "Dell", 2000.5, 'E');
	afisare(l);
	modificaPret(&l, 1000);
	afisare(l);
	dezalocare(&l);
	afisare(l);
	return 0;
}