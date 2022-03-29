#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Produs
{
	int cantitate, cumparate;
	char nume_produs[50], tara_produs[50];
}Produs;

//verificarea existentei fisierului de tip txt
int file_exists() {
	FILE* fp;
	if (fp = fopen("StockProduse.txt", "r")) {
		fclose(fp);
		return 1;
	}
	return 0;
}

void reset() {
	printf("\033[0m");
}

void red() {
	printf("\033[0;31m");
}

void yellow() {
	printf("\033[1;33m");
}

void green() {
	printf("\033[1;32m");
}

void capitalize_letter(char str[100]) {
	int i;
	/*Vom afisa doar prima litera a cuvantului cu majuscule*/
	for (i = 0; str[i] != '\0'; i++)
	{
		/*verificam daca prima litera e minuscula*/
		if (i == 0)
		{
			if ((str[i] >= 'a' && str[i] <= 'z'))
				/*Vom scadea valoarea cu -32 pentru a o face majuscula*/
				str[i] = str[i] - 32;
			continue; 
		}
		if (str[i] == ' ') /*Verificam daca a vem spatii*/
		{
			++i;
			if (str[i] >= 'a' && str[i] <= 'z')
			{
				str[i] = str[i] - 32; 
				continue; 
			}
		}
		else
		{
			/*Restul literelor vor fi minuscule*/
			if (str[i] >= 'A' && str[i] <= 'Z')
				str[i] = str[i] + 32; 
		}
	}
}

/*Declarare prototip functii*/
int check_existance(char produs[100], char tara[100]); //verificarea existentei unui produs
void citire_produs();
void afisare_produse();
void stergere_produs();
void cautare_produs();
void editare_produs();
void cumparare_produs();
void actualizare_stoc();

int main()
{
	int contor = 1;
	int optiune;
	while (contor != 0)
	{
		yellow();
		printf("\n0. Iesire.\n");
		printf("1. Adaugare produs\n");
		printf("2. Afisare produse\n");
		printf("3. Stergere produs\n");
		printf("4. Cautare produs\n");
		printf("5. Editare produs\n");
		printf("6. Cumparare produs\n");
		printf("7. Actualizare stoc\n");
		printf("Optiune: ");
		reset();
		(void)scanf("%d", &optiune);
		printf("\n");
		switch (optiune)
		{
		case 0:
			contor = 0;
			break;
		case 1:
			citire_produs();
			break;
		case 2:
			afisare_produse();
			break;
		case 3:
			stergere_produs();
			break;
		case 4: 
			cautare_produs();
			break;
		case 5:
			editare_produs();
			break;
		case 6:
			cumparare_produs();
			break;
		case 7:
			actualizare_stoc();
			break;
		default: 
			red();
			printf("Optiunea nu exista!\n");
			reset();
			break;
		}
	}
	return 0;
}

int check_existance(char produs[100], char tara[100]) {
	Produs p1;
	FILE* fp;
	int found = 0;
	fp = fopen("StockProduse.txt", "r");
	//Parcurgerea fisierului de tip txt
	while (fread(&p1, sizeof(Produs), 1, fp)) {
		//verificarea in cazul existentei produsului pe care dorim sa il introducem
		if ((stricmp(p1.nume_produs, produs) == 0) && (stricmp(p1.tara_produs, tara) == 0)) {
			found = 1;
		}
	}
	fclose(fp);
	if (found == 1)
		return 1;
	else
		return 0;
}

void citire_produs() {
	Produs* p;
	FILE* fp;
	int numar_produse, i;
	int check = file_exists();
	//Avem optiunea de a selecta cate produse dorim sa introducem odata
	printf("Introduceti numarul de produse: ");
	(void)scanf("%d", &numar_produse);
		p = (Produs*)calloc(numar_produse, sizeof(Produs));
		/*In cazul in care fisierul nu exista, acesta va fi creat*/
		if (check == 0)
			fp = fopen("StockProduse.txt", "w");
		/*In cazul in care fisierul exista si avem produse in Stock,
		noul produs va fi adaugat, fara a sterge produsele anterioare*/
		else
			fp = fopen("StockProduse.txt", "a");

		for (i = 0; i < numar_produse; i++) {
			(void)getc(stdin);
			printf("Produs: ");
			(void)scanf("%[^\n]s", p[i].nume_produs);
			capitalize_letter(p[i].nume_produs);
			(void)getc(stdin);
			printf("Tara: ");
			(void)scanf("%[^\n]s", p[i].tara_produs);
			capitalize_letter(p[i].tara_produs);
			(void)getc(stdin);
			printf("Cantitate: ");
			(void)scanf("%d", &p[i].cantitate);
			/*In cazul in care cantitatea pe care dorim sa o introducem
			este mai mica sau egala decat 0, programul ne va obliga sa
			reintroducem o valoare pozitiva nenula*/
			while (p[i].cantitate <= 0) {
				printf("\nCantitatea trebuie sa fie mai mare decat 0!\n");
				printf("Cantitate: ");
				(void)scanf("%d", &p[i].cantitate);
			}
			printf("\n");
			/*Vom initializa cu 0 cantitatea cumparata pentru fiecare produs,
			aceasta poate fi modificata prin cumpararea produsului respectiv*/
			p[i].cumparate = 0;
			/*Apelam functia pentru verificarea existentei in stock a produsului*/
			int ch = check_existance(p[i].nume_produs, p[i].tara_produs);
			if (ch == 0)
				fwrite(&p[i], sizeof(Produs), 1, fp);
			else {
				red();
				printf("\nProdusul este deja in stoc!\n");
				reset();
			}
		}
		fclose(fp);
	}

void afisare_produse() {
	Produs* p, p1;
	FILE* fp;
	int i, j;
	int check = file_exists();
	if (check == 1) {
		fp = fopen("StockProduse.txt", "r");
		fseek(fp, 0, SEEK_END);
		int n = ftell(fp) / sizeof(Produs);
		p = (Produs*)calloc(n, sizeof(Produs));
		rewind(fp);
		for (i = 0; i < n; i++)
			if (p != NULL)
				fread(&p[i], sizeof(Produs), 1, fp);
		/*Vom parcurge fiecare produs din fisier
		pentru a le putea sorta alfabetic*/
		for (i = 0; i < n; i++) {
			for (j = i + 1; j < n; j++) {
				if (p != NULL) {
					if (strcmp(p[i].nume_produs, p[j].nume_produs) > 0) {
						p1 = p[i];
						p[i] = p[j];
						p[j] = p1;
					}
				}
			}
		}
		fp = fopen("StockProduse.txt", "r");
		printf("\n------------------------------------------------------------------------\n");
		printf("|%-20s%-20s%-20s%s |\n", "Produs", "Tara", "Cantitate", "Cumparate");
		printf("------------------------------------------------------------------------\n");
		/*Vom parcurge fisierul si vom afisa fiecare produs din stoc*/
		for (i = 0; i < n; i++) {
			if (p != NULL) {
				if (p[i].cantitate == 0) {
					/*In cazul in care cantitatea ramasa in stock este 0,
					vom afisa "stoc epuizat pentru produsul respectiv"*/
					printf("%-20s%-20s", p[i].nume_produs, p[i].tara_produs);
					red();
					printf("%-24s", "Stock epuizat");
					reset();
					if (p[i].cumparate > 0) {
						green();
						printf("%d\n", p[i].cumparate);
						reset();
					}
					else
						printf("%d\n", p[i].cumparate);
				}
				else {
					printf("%-20s%-24s%-20d", p[i].nume_produs, p[i].tara_produs, p[i].cantitate);
					if (p[i].cumparate > 0) {
						green();
						printf("%d\n", p[i].cumparate);
						reset();
					}
					else
						printf("%d\n", p[i].cumparate);
				}
			}
		}
		fclose(fp);
	}
	/*In cazul in care fisierul nu exista, citirea este imposibila,
	astfel vom afisa mesajul de mai jos*/
	else {
		red();
		printf("Fisierul nu exista!\n");
		reset();
	}
}

void stergere_produs() {
	/*Pentru stergerea unui produs,
	ne vom folosi de un fisier ajutator, "fp1"*/
	Produs p1;
	FILE* fp, * fp1;
	char numeP[100], taraP[100];
	int found = 0;
	/*Verificarea existentei fisierului*/
	int check = file_exists();
	if (check == 1) {
		fp = fopen("StockProduse.txt", "r");
		fp1 = fopen("NewStock.txt", "w");
		/*Stergerea si face prin cautarea produsului
		dupa nume si tara de provenienta*/
		(void)getc(stdin);
		printf("Introduceti numele produsului: ");
		(void)scanf("%[^\n]s", numeP);
		(void)getc(stdin);
		printf("Introduceti tara produsului: ");
		(void)scanf("%[^\n]s", taraP);
		while (fread(&p1, sizeof(Produs), 1, fp)) {
			if (numeP[strlen(numeP)] == '\0' && taraP[strlen(taraP)] == '\0') {
				if ((stricmp(p1.nume_produs, numeP) == 0) && (stricmp(p1.tara_produs, taraP) == 0)) {
					found = 1;
				}
				else
					fwrite(&p1, sizeof(Produs), 1, fp1);
			}
		}
		/*Ne vom folosi din nou de un fisier nou de tip txt
		pentru a realiza stergerea produsului din stock*/
		fclose(fp);
		fclose(fp1);
		if (found) {
			fp1 = fopen("NewStock.txt", "r");
			fp = fopen("StockProduse.txt", "w");

			while (fread(&p1, sizeof(Produs), 1, fp1)) {
				fwrite(&p1, sizeof(Produs), 1, fp);
			}

			fclose(fp);
			fclose(fp1);
		}
		/*In cazul in care produsul nu exista,
		se va afisa mesajul de mai jos*/
		if (!found) {
			red();
			printf("\nProdusul nu exista!\n");
			reset();
		}
	}
	else {
		red();
		printf("\nFisierul nu exista!\n");
		reset();
	}
}

void cautare_produs() {
	Produs p1;
	FILE* fp;
	char numeP[100], taraP[100];
	int found = 0;
	/*Verificarea existentei fisierului*/
	int check = file_exists();
	if (check == 1) {
		/*Vom introduce numele si tara de provenienta a produsului, 
		pentru cautarea existentei acestuia in stock*/
		fp = fopen("StockProduse.txt", "r");
		(void)getc(stdin);
		printf("Introduceti numele produsului: ");
		(void)scanf("%[^\n]s", numeP);
		(void)getc(stdin);
		printf("Introduceti tara produsului: ");
		(void)scanf("%[^\n]s", taraP);
		/*Vom parcurge fisierul, iar in cazul gasirii produsului, il vom afisa*/
		while (fread(&p1, sizeof(Produs), 1, fp)) {
			/*Cautarea nu este Case-Sensitive*/
			if (numeP[strlen(numeP)] == '\0' && taraP[strlen(taraP)] == '\0') {
				if (stricmp(p1.nume_produs, numeP) == 0 && stricmp(p1.tara_produs, taraP) == 0) {
					found = 1;
					printf("\n-----------------------------------------------------------------------\n");
					printf("|%-20s%-20s%-20s%s|\n", "Produs", "Tara", "Cantitate", "Cumparate");
					printf("-----------------------------------------------------------------------\n");
					if (p1.cantitate == 0) {
						printf("%-20s%-20s", p1.nume_produs, p1.tara_produs);
						red();
						printf("%-25s", "Stock epuizat");
						reset();
					}
					else
						printf("%-21s%-23s%-20d", p1.nume_produs, p1.tara_produs, p1.cantitate);
					if (p1.cumparate > 0) {
						green();
						printf("%-20d\n", p1.cumparate);
						reset();
					}
					else
						printf("%-20d\n", p1.cumparate);
				}
				
			}
		}
		printf("\n");
		/*In cazul in care nu am gasit produsul,
		vom afisa mesajul de mai jos*/
		if (!found) {
			red();
			printf("Produsul nu exista!\n");
			reset();
		}
		fclose(fp);
	}
	/*In cazul in care fisierul nu exista vom afisa mesajul de mai jos*/
	else {
		red();
		printf("\nFisierul nu exista!\n");
		reset();
	}
}

void editare_produs() {
	/*Pentru editarea unui produs,
	ne vom folosi de un fisier ajutator, "fp1"*/
	Produs p1;
	FILE* fp, * fp1;
	char numeP[100], taraP[100];
	int found = 0;
	/*Apelarea functiei pentru determinarea existentei fisierului*/
	int check = file_exists();
	if (check == 1) {
		fp = fopen("StockProduse.txt", "r");
		fp1 = fopen("NewStock.txt", "w");
		/*Introducerea numelui si tarii de provenienta 
		a produsului pe care dorim sa il editam*/
		(void)getc(stdin);
		printf("Introduceti numele produsului: ");
		(void)scanf("%[^\n]s", numeP);
		(void)getc(stdin);
		printf("Introduceti tara produsului: ");
		(void)scanf("%[^\n]s", taraP);
		/*Parcurgem fisierul pentru a cauta produsul respectiv*/
		while (fread(&p1, sizeof(Produs), 1, fp)) {
			/*Cautarea nu ete Case-Sensitive*/
			if (numeP[strlen(numeP)] == '\0' && taraP[strlen(taraP)] == '\0') {
				if (stricmp(p1.nume_produs, numeP) == 0 && stricmp(p1.tara_produs, taraP) == 0) {
					found = 1;
					/*In cazul gasirii produsului,
					vom putea modifica: numele, tara si cantitatea acestuia*/
					(void)getc(stdin);
					printf("Noul produs: ");
					(void)scanf("%[^\n]s", p1.nume_produs);
					(void)getc(stdin);
					printf("Noua tara: ");
					(void)scanf("%[^\n]s", p1.tara_produs);
					(void)getc(stdin);
					printf("Noua cantitate: ");
					(void)scanf("%d", &p1.cantitate);
					printf("\n");
					p1.cumparate = 0;
					/*Apelam functia pentru a transforma numele
					si tara fiecarui produs in format potrivit*/
					capitalize_letter(p1.nume_produs);
					capitalize_letter(p1.tara_produs);
				}
			}
			/*Pentru a modifica produsul,
			am folosit un nou fisier txt,
			pentru a introduce noile valori,
			acestea urmand a fi updatate in fisierul initial*/
			fwrite(&p1, sizeof(Produs), 1, fp1);
		}
		fclose(fp);
		fclose(fp1);
		if (found) {
			fp1 = fopen("NewStock.txt", "r");
			fp = fopen("StockProduse.txt", "w");

			while (fread(&p1, sizeof(Produs), 1, fp1)) {
				fwrite(&p1, sizeof(Produs), 1, fp);
			}
			fclose(fp);
			fclose(fp1);
		}
		/*In cazul in care produsul nu este gasit,
		editarea este imposibila,
		iar mesajul de mai jos va fi afisat*/
		if (!found) {
			red();
			printf("\nProdusul nu exista!\n");
			reset();
		}
	}
	/*Mesajul de mai jos va fi afisat in cazul
	inexistentei fisierului cautat*/
	else {
		red();
		printf("\nFisierul nu exista!\n");
		reset();
	}
}

void cumparare_produs() {
	/*Pentru a putea edita cantitatea cumparata
	ne vom folosi de un fisier ajutator, "fp1"*/
	Produs p1;
	FILE* fp, * fp1;
	char numeP[100], taraP[100];
	int found = 0, cump;
	/*Verificarea existentei fisierului*/
	int check = file_exists();
	if (check == 1) {
		fp = fopen("StockProduse.txt", "r");
		fp1 = fopen("NewStock.txt", "w");
		/*La fel ca in celelalte functii,
		cautarea se face dupa nume si tara de provenienta */
		(void)getc(stdin);
		printf("Introduceti numele produsului: ");
		(void)scanf("%[^\n]s", numeP);
		(void)getc(stdin);
		printf("Introduceti tara produsului: ");
		(void)scanf("%[^\n]s", taraP);
		/*Vom parcurge fisierul pentru a gasi produsul dorit*/
		while (fread(&p1, sizeof(Produs), 1, fp)) {
			if (numeP[strlen(numeP)] == '\0' && taraP[strlen(taraP)] == '\0') {
				if (stricmp(p1.nume_produs, numeP) == 0 && stricmp(p1.tara_produs, taraP) == 0) {
					found = 1;
					/*In cazul in care stocul este epuizat
					pentru un anumit produs,
					vom afisa mesajul de mai jos*/
					if (p1.cantitate == 0) {
						red();
						printf("\nProdus indisponibil!\n");
						reset();
					}
					else {
						/*In cazul in care produsul este gasit,
					putem alege cantitatea pe care dorim sa o cumparam*/
						printf("\nIntroduceti cantitatea pe care doriti sa o cumparati: ");
						(void)scanf("%d", &cump);
						/*Cantitatea trebuie sa fie pozitiva, nenula*/
						while (cump <= 0) {
							printf("\nCantitatea este prea mica!\n");
							printf("Introduceti o valoare mai mare decat 0!\n");
						}
						/*Cantitatea pe care dorim sa o cumparam trebuie
						sa fie mai mica sau egala cu cea disponibila in stock*/
						while (cump > p1.cantitate) {
							printf("\nCantitatea este prea mare.\n");
							printf("\nIntroduceti o cantitate mai mica: ");
							(void)scanf("%d", &cump);
						}
						/*Dupa ce o anumita cantitate a fost cumparata,
						numarul de produse de acelasi fel va fi updatat,
						prin eliminarea cantitatii cumparate*/
						if (cump > 0) {
							p1.cantitate = p1.cantitate - cump;
							p1.cumparate = p1.cumparate + cump;
						}
					}
				}
			}
			fwrite(&p1, sizeof(Produs), 1, fp1);
		}
		fclose(fp);
		fclose(fp1);
		if (found) {
			fp1 = fopen("NewStock.txt", "r");
			fp = fopen("StockProduse.txt", "w");

			while (fread(&p1, sizeof(Produs), 1, fp1)) {
				fwrite(&p1, sizeof(Produs), 1, fp);
			}

			fclose(fp);
			fclose(fp1);
		}
		/*In cazul in care produsul nu exista
		se va afisa mesajul de mai jos*/
		if (!found) {
			red();
			printf("\nProdusul nu exista!\n");
			reset();
		}
	}
	else {
		red();
		printf("\nFisierul nu exista!\n");
		reset();
	}
} 

void actualizare_stoc() {
	Produs p1;
	FILE* fp, * fp1;
	char numeP[100], taraP[100];
	int found = 0, cump;
	/*Verificarea existentei fisierului*/
	int check = file_exists();
	if (check == 1) {
		fp = fopen("StockProduse.txt", "r");
		fp1 = fopen("NewStock.txt", "w");
		/*La fel ca in celelalte functii,
		cautarea se face dupa nume si tara de provenienta */
		(void)getc(stdin);
		printf("Introduceti numele produsului: ");
		(void)scanf("%[^\n]s", numeP);
		(void)getc(stdin);
		printf("Introduceti tara produsului: ");
		(void)scanf("%[^\n]s", taraP);
		while (fread(&p1, sizeof(Produs), 1, fp)) {
			if (numeP[strlen(numeP)] == '\0' && taraP[strlen(taraP)] == '\0') {
				if (stricmp(p1.nume_produs, numeP) == 0 && stricmp(p1.tara_produs, taraP) == 0) {
					found = 1;
					/*In cazul in care produsul este gasit,
					vom putea modifica numarul de produse
					prezente in stoc*/
					printf("Introduceti noua cantitate: ");
					(void)getc(stdin);
					(void)scanf("%d", &cump);
					/*In cazul in care valoarea introdusa
					este mai mica sau egala cu 0
					va trebui introdusa o alta valoare*/
					while (cump <= 0) {
						printf("\nValoarea este prea mica!\n");
						printf("Introduceti o noua valoare: ");
						(void)scanf("%d", &cump);
					}
					/*Vom adauga nou cantitate la 
					cea deja existenta*/
					p1.cantitate = p1.cantitate + cump;
				}
			}
			fwrite(&p1, sizeof(Produs), 1, fp1);
		}
		fclose(fp);
		fclose(fp1);
		if (found) {
			fp1 = fopen("NewStock.txt", "r");
			fp = fopen("StockProduse.txt", "w");

			while (fread(&p1, sizeof(Produs), 1, fp1)) {
				fwrite(&p1, sizeof(Produs), 1, fp);
			}

			fclose(fp);
			fclose(fp1);
		}
		/*In cazul in care produsul nu este
		gasit, se va afisa mesajul de mai jos*/
		if (!found) {
			red();
			printf("\nProdusul nu exista!\n");
			reset();
		}
	}
	else {
		red();
		printf("\nFisierul nu exista!\n");
		reset();
	}
}