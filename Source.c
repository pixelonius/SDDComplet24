#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>

struct student {
	char* nume;
	int cod;
};

typedef struct student student;
typedef struct nodLista nod;

struct nodLista {
	student info;
	nod* next;
};

student initializareStudent(int cod, char* nume) {
	student stud;
	stud.cod = cod;
	stud.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(stud.nume, nume);
	return stud;
}

void afisareStudent(student stud) {
	printf("Studentul %s are codul %d", stud.nume, stud.cod);
}

//lista simpla
//inserare
nod* inserareInceput(nod* cap, student stud) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->next = cap;
	nou->info = initializareStudent(stud.cod, stud.nume);
	return nou;
}

nod* inserareSfarsit(nod* cap, student stud) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = initializareStudent(stud.cod, stud.nume);
	nou->next = NULL;
	if (cap == NULL) {
		return nou;
	}
	else {
		nod* temp = cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
		return cap;
	}
}
void afisareLista(nod* cap) {
	while (cap) {
		afisareStudent(cap->info);
		cap = cap->next;
	}
}
//lista dubla
typedef struct nodDublu nodDublu;
struct nodDublu {
	student info;
	nodDublu* next;
	nodDublu* prev;
};

void inserareInceputDubla(nodDublu** cap, nodDublu** coada, student stud) {
	nodDublu* nou = (nodDublu*)malloc(sizeof(nodDublu));
	nou->prev = NULL;
	nou->next = NULL;
	nou->info = initializareStudent(stud.cod, stud.nume);
	if (*cap == NULL) {
		*cap = nou;
		*coada = nou;
	}
	else {
		nou->next = *cap;
		nou->prev=*coada;
		(*cap)->prev = nou;
		(*coada)->next = nou;
		*cap = nou;

	}
}

void afisareDubla(nodDublu* cap, nodDublu* coada) {
	nodDublu* prim = cap;
	afisareStudent(cap->info);
	cap = cap->next;
	while (cap != prim) {
		afisareStudent(cap->info);
		cap = cap->next;
	}
}
//stiva
void push(nod** varf, student s) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->next = NULL;
	nou->info.cod = s.cod;
	nou->info.nume = (char*)malloc(sizeof(char*) * (strlen(s.nume) + 1));
	strcpy(nou->info.nume, s.nume);
	if (*varf == NULL) {
		*varf = nou;
	}
	else {
		nou->next = *varf;
		*varf = nou;
	}
}

int pop(nod** varf, student* s) {
	if (*varf == NULL)
		return -1;
	else {
		s->cod = (*varf)->info.cod;
		s->nume = (char*)malloc(sizeof(char*) * (strlen((*varf)->info.nume) + 1));
		strcpy(s->nume, (*varf)->info.nume);
		nod* temp = *varf;
		*varf = (*varf)->next;
		free(temp->info.nume);
		free(temp);
		return 0;
	}
}

//coada
void put(nod** prim, nod** ultim, student s) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info.cod = s.cod;
	nou->info.nume = (char*)malloc(sizeof(char*) * (strlen(s.nume) + 1));
	strcpy(nou->info.nume, s.nume);
	nou->next = NULL;
	if (*prim == NULL && *ultim == NULL) {
		*prim = nou;
		*ultim = nou;
	}
	else {
		(*ultim)->next = nou;
		*ultim = nou;
	}

}

int get(nod** prim, nod** ultim, student* s) {
	if (*prim != NULL && *ultim != NULL) {
		s->cod = (*prim)->info.cod;
		s->nume = (char*)malloc(sizeof(char*) * (strlen((*prim)->info.nume) + 1));
		strcpy(s->nume, (*prim)->info.nume);
		nod* temp = *prim;
		*prim = (*prim)->next;
		free(temp->info.nume);
		free(temp);
		return 0;
	}
	else {
		if (*prim == NULL) {
			*ultim = NULL;
			return -1;
		}
	}
}

//hashtable chaining
typedef struct hashTable hash;
struct hashTable {
	nod** vector;
	int dim;
};

//initializare tabela
hash initializareTabela(int dimensiune) {
	hash tabela;
	tabela.dim = dimensiune;
	tabela.vector = (nod**)malloc(sizeof(nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		tabela.vector[i] = NULL;
	}
	return tabela;
}
int calculHash(int cod, int dimensiune) {
	if (dimensiune > 0) {
		return cod % dimensiune;
	}
	return -1;
 }

void inserareFinalHash(nod** cap, student stud) {
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->next = NULL;
	nou->info.cod = stud.cod;
	nou->info.nume = (char*)malloc(sizeof(char) * (strlen(stud.nume) + 1));
	strcpy(nou->info.nume, stud.nume);
	if (*cap) {
		nod* tmp = *cap;
		while (tmp->next) {
			tmp = tmp->next;
		}
		tmp->next = nou;
	}
	else {
		*cap = nou;
	}
}

void inserareInTabela(hash tabela, student stud) {
	if (tabela.dim > 0) {
		int poz = calculHash(stud.cod, tabela.dim);
			inserareFinalHash(&tabela.vector[poz], stud);
	}
}

student cautaStudentDupaCod(int cod, const char* nume, hash tabela) {
	int poz = calculHash(cod, tabela.dim);
	if (poz >= 0 && poz < tabela.dim) {
		nod* cautare = tabela.vector[poz];
		while (cautare) {
			if (cautare->info.cod == cod) {
				if (strcmp(cautare->info.nume, nume) == 0) {
					break;
				}
			}
			cautare = cautare->next;
		}
		if (cautare)
			return cautare->info;
		else return initializareStudent(-1, "");
	}
	else return initializareStudent(-1, "");
}

void deleteByCodHT(nod** cap, student stud) {
	if (
		(*cap)->info.cod == stud.cod && strcmp((*cap)->info.nume, stud.nume) == 0 //daca este primul element
		) {
		nod* tmp = *cap;
		*cap = (*cap)->next;
		free(tmp->info.nume);
		free(tmp);
		tmp = NULL;
	}
	else {
		nod* tmp = *cap;
		while (tmp->next->next && tmp->next->info.cod != stud.cod && strcmp(tmp->next->info.nume, stud.nume) == 0) {
			tmp = tmp->next;
		}
		if (!tmp->next->next && tmp->next->info.cod == stud.cod && strcmp(tmp->next->info.nume, stud.nume) == 0) {//daca este ultimul
			nod* sters = tmp->next;
			tmp->next = NULL;
			free(sters->info.nume);
			free(sters);
			sters = NULL;
			return;
		}
		if (tmp->next->next && tmp->next->info.cod == stud.cod && strcmp(tmp->next->info.nume, stud.nume) == 0) {//daca este la miijloc
			nod* sters = tmp->next;
			tmp->next = tmp->next->next;
			free(sters->info.nume);
			free(sters);
			sters = NULL;
			return;
		}
	}

}

void deleteFromHash(hash* tabela, student stud) {
	int pozitie = calculHash(stud.cod, tabela->dim);
	if (pozitie >= 0 && pozitie < tabela->dim) {
		nod* cautare = tabela->vector[pozitie];
		while (cautare) {
			if (cautare->info.cod == stud.cod) {
				if (strcmp(cautare->info.nume, stud.nume) == 0) {
					deleteByCodHT(&tabela->vector[pozitie], stud);// cod custom de steregere cu mici modificari, maybe another time
					break;
				}
			}
			cautare = cautare->next;
		}
	}
}

void afisareHashTabela(hash tabela) {
	for (int i = 0; i < tabela.dim; i++) {
		if (tabela.vector[i] != NULL) {
			printf("Clusterul: %d", i + 1);
			afisareLista(tabela.vector[i]);
			printf("\n");
		}
	}
}



//hashtable linear probing
//heap
struct apelUrgenta {
	int prioritate;
	char* numeApelant;
};
typedef struct apelUrgenta apelUrgenta;
typedef struct heap heap;
struct heap {
	apelUrgenta* vect;
	int dimensiune;
};

heap initializareHeap(int nr) {
	heap h;
	h.vect = malloc(nr * sizeof(apelUrgenta));
	h.dimensiune = nr;
	return h;
}
//se sorteaza pe prioritate, cel mai mare fiind radacina
void filtrare(heap h, int index) {
	int indexMax = index;
	int indexS = 2 * index + 1;
	int indexD = 2 * index + 2;

	if (indexS < h.dimensiune && h.vect[indexS].prioritate > h.vect[indexMax].prioritate) 
		indexMax = indexS;
	if (indexD<h.dimensiune && h.vect[indexD].prioritate > h.vect[indexMax].prioritate)
		indexMax = indexD;

	if (indexMax != index) {
		apelUrgenta tmp = h.vect[index];
		h.vect[index] = h.vect[indexMax];
		h.vect[indexMax] = tmp;
		filtrare(h, indexMax);
	}
}

//inserare
void inserareHeap(heap* h, apelUrgenta apel) {
	apelUrgenta* tmp = (apelUrgenta*)malloc(sizeof(apelUrgenta) * (h->dimensiune + 1));
	for (int i = 0; i < h->dimensiune; i++) {
		tmp[i] = h->vect[i];
	}
	h->dimensiune++;
	free(h->vect);
	h->vect = (apelUrgenta*)malloc(sizeof(apelUrgenta) * h->dimensiune);
	for (int i = 0; i < h->dimensiune - 1; i++) {
		h->vect[i] = tmp[i];
	}
	h->vect[h->dimensiune - 1] = apel;
	for (int i = h->dimensiune / 2-1; i >= 0; i--) {
		filtrare(*h, i);
	}
	free(tmp);
	
};

////extragere fara eliberarea memoriei
//apelUrgenta extragereMxim(heap* h) {
//	if (h->nrElem > 0) {
//		apelUrgenta tmp;
//		tmp = h->vect[0];
//		h->vect[0] = h->vect[h->nrElem - 1];
//		h->vect[h->nrElem - 1] = tmp;
//		//in tmp avem acum cel mai mare element. in primul element este acum ultimul
//		h->nrElem--;
//
//		for (int i = (h->nrElem - 1) / 2; i >= 0; i--) {
//			filtrare(*h, i);
//		}
//		return tmp;
//	}
//	//else return element null;
//}

//extragere cu modificarea memoriei
apelUrgenta extragereMaxim(heap* h) {
	if (h->dimensiune > 0) {
		apelUrgenta* vect1 = (apelUrgenta*)malloc(sizeof(apelUrgenta) * (h->dimensiune - 1));
		apelUrgenta tmp = h->vect[0];
		h->vect[0] = h->vect[h->dimensiune - 1];
		h->vect[h->dimensiune-1] = tmp;
		//in tmp maximul

		//copiem tot mai putin ultima valoare din vectorul actual intr-unul temporar
		for (int i = 0; i < h->dimensiune - 1; i++) {
			vect1[i] = h->vect[i];
		}
		//micsoram marimea si eliberam vectorul vechi, apoi copiem elementele ramase inapoi
		h->dimensiune--;
		free(h->vect);
		h->vect = (apelUrgenta*)malloc(sizeof(apelUrgenta) * h->dimensiune);
		for (int i = 0; i < h->dimensiune; i++) {
			h->vect[i] = vect1[i];
		}
		for (int i = (h->dimensiune - 1) / 2; i >= 0; i--) {
			filtrare(*h, i);
		}
		return tmp;

	}
}

void afisareHeap(heap h) {
	for (int i = 0; i < h.dimensiune; i++) {
		printf("Proritate: %d, ", h.vect[i].prioritate);
		printf("Apelant: %s\n", h.vect[i].numeApelant);
	}
}

// dezalocare heap
void dezalocareHeap(heap* h) {
	for (int i = 0; i < h->dimensiune; i++) {
		free(h->vect[i].numeApelant);
	}
	free(h->vect);
	h->vect = NULL;
	h->dimensiune = 0;
}

//lol
typedef struct nodLS nodLS;
struct nodLS {
	student info;
	nodLS* next;
};

typedef struct nodLP nodLP;
struct nodLP {
	nodLS* info;
	nodLP* next;
};

void inserareLS(nodLS** cap, student stud) {
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->next = NULL;
	nou->info.cod = stud.cod;
	nou->info.nume = (char*)malloc(sizeof(char) * (sizeof(stud.nume) + 1));
	strcpy(nou->info.nume, stud.nume);
	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		nodLS* tmp = *cap;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = nou;
	}
}

void inserareLP(nodLP** capLP, nodLS* capLS) {
	nodLP* nou = (nodLP*)malloc(sizeof(nodLP));
	nou->next = NULL;
	nou->info = capLS;
	if (*capLP == NULL) {
		*capLP = nou;
	}
	else {
		nodLP* tmp = *capLP;
		while (tmp->next) {
			tmp = tmp->next;
		}
		tmp->next = nou;
	}
}
void traversareLS(nodLS* capLS) {
	nodLS* tmp = capLS;
	while (tmp) {
		//do something
		tmp = tmp->next;
	}
}

void traversareLP(nodLP* capLP) {
	nodLP* tmp = capLP;
	while (tmp) {
		//do something
		tmp = tmp->next;
	}
}

//BST
typedef struct BST BST;
struct BST {
	student stud;
	BST* left;
	BST* right;
};

BST* creareNod(student stud) {
	BST* nou = (BST*)malloc(sizeof(BST));
	nou->stud= stud;
	nou->left = NULL;
	nou->right = NULL;
	return nou;
}

BST* inserareBST(BST* radacina, student stud) {
	if (radacina != NULL) {
		if (stud.cod < radacina->stud.cod) {
			radacina->left = inserareBST(radacina->left, stud);
			
		}
		else if (stud.cod > radacina->stud.cod) {
			radacina->right = inserareBST(radacina->right, stud);
		}
		return radacina;

	}
	else return creareNod(stud);
}
void preordine(BST* radacina) {
	
	if (radacina == NULL)
		return;
	afisareStudent(radacina->stud);
	preordine(radacina->left);
	preordine(radacina->right);
}

void inordine(BST* radacina) {
	inordine(radacina->left);
	//actiune
	inordine(radacina->right);
}
void postordine(BST* radacina) {
	postordine(radacina->left);
	postordine(radacina->right);
	//actiune
}

//AVL -> este un BST echilibrat
BST* rotireDreapta(BST* rad) {
	if (rad) {
		BST* aux = rad->left;
		rad->left = aux->right;
		aux->right = rad;
		return aux;
	}
	return rad;
}

BST* rotireStanga(BST* rad) {
	if (rad) {
		BST* aux = rad->right;
		rad->right = aux->left;
		aux->left = rad;
		return aux;
	}
	return rad;
}

int calculInaltime(BST* radacina) {
	if (radacina) {
		int inaltimeSt = calculInaltime(radacina->left);
		int inaltimeDr = calculInaltime(radacina->right);
		if (inaltimeSt > inaltimeDr)
			return 1 + inaltimeSt;
		return 1 + inaltimeDr;
	}
	return 0;
}

int gradEchilibru(BST* radacina) {
	int inaltimeSubarboreStang = calculInaltime(radacina->left);
	int inaltimeSubarboreDrept = calculInaltime(radacina->right);
	return inaltimeSubarboreStang - inaltimeSubarboreDrept;
}

void inserareAVL(BST** radacina, student stud) {
	if (*radacina) {
		if ((*radacina)->stud.cod > stud.cod)
			inserareAVL(&((*radacina)->left), stud);
		else inserareAVL(&((*radacina)->left), stud);
		int gradEq = gradEchilibru(*radacina);
		if (gradEq == 2) {//dezechilibru in stanga
			int gradCopil = gradEchilibru((*radacina)->left);
			if (gradCopil == 1) {
				(*radacina)->left = rotireStanga((*radacina)->left);
			}
			*radacina = rotireDreapta((*radacina));

		}
		else if (gradEq == -2) {
			int gradCopil = gradEchilibru((*radacina)->right);
			if (gradCopil == 1) {
				(*radacina)->right = rotireDreapta((*radacina)->right);

			}
			*radacina = rotireStanga((*radacina));
		}
	}
	else {
		*radacina = creareNod(stud);;
	}
}
//TODO
//Grafuri
//hash cu linear probing

void main() {
	
//FILE* f = fopen("nume_fisier", "r");
//	if (f != NULL) {
//		char buffer[100];
//		char delimitator[] = ",\n";
//		while (fgets(buffer, 100, f) != NULL) {
//			char* token;
//			token = strtok(buffer, delimitator);
//			//in token avem cuvintele si le prelucram
//			//trecem la urmatorul cuvant
//			token = strtok(NULL, delimitator);
//			}
//		fclose(f);
//		}
	
nod* listaSimpla = NULL;
nodDublu* listaDublaCap = NULL;
nodDublu* listaDublaCoada = NULL;
student stud;
stud.cod = 1;
stud.nume = (char*)malloc(sizeof(char) * strlen("Maria") + 1);
strcpy(stud.nume, "Maria");
printf("\nLista simpla----------------------\n");
listaSimpla = inserareInceput(listaSimpla, stud);
listaSimpla = inserareInceput(listaSimpla, initializareStudent(2, "Alexandru"));
listaSimpla = inserareSfarsit(listaSimpla, initializareStudent(3, "Marius"));
afisareLista(listaSimpla);

printf("\nLista Dubla ---------------------------\n");
inserareInceputDubla(&listaDublaCap, &listaDublaCoada, stud);
inserareInceputDubla(&listaDublaCap, &listaDublaCoada, initializareStudent(2, "Alexandru"));
inserareInceputDubla(&listaDublaCap, &listaDublaCoada, initializareStudent(3, "Marius"));
afisareDubla(listaDublaCap, listaDublaCoada);

printf("\nStiva--------------\n");
nod* capStiva = NULL;
push(&capStiva, stud);
push(&capStiva, initializareStudent(2, "Alexandru"));
push(&capStiva, initializareStudent(3, "Mircea"));
student rez;
pop(&capStiva, &rez);
pop(&capStiva, &rez);
afisareStudent(rez);

printf("\nCoada--------------\n");
nod* primCoada = NULL;
nod* ultimCoada = NULL;
put(&primCoada, &ultimCoada, stud);
put(&primCoada, &ultimCoada, initializareStudent(2, "Alexandru"));
put(&primCoada, &ultimCoada, initializareStudent(3, "Mircea"));
get(&primCoada, &ultimCoada, &rez);
afisareStudent(rez);

printf("\nHashtable-----------\n");
hash tabela;
tabela = initializareTabela(3);
inserareInTabela(tabela, stud);
inserareInTabela(tabela, initializareStudent(2, "Alexandru"));
inserareInTabela(tabela, initializareStudent(3, "Mircea"));
afisareHashTabela(tabela);

rez = cautaStudentDupaCod(2, "Alexandru", tabela);
afisareStudent(rez);

printf("\nHeap----------------\n");
apelUrgenta apel1;
apel1.numeApelant = (char*)malloc(sizeof(char) * strlen("Marius") + 1);
strcpy(apel1.numeApelant, "Marius");
apel1.prioritate = 1;
apelUrgenta apel2;
apel2.numeApelant = (char*)malloc(sizeof(char) * strlen("Nicu") + 1);
apel2.prioritate = 2;
strcpy(apel2.numeApelant, "Nicu");
apelUrgenta apel3;
apel3.numeApelant = (char*)malloc(sizeof(char) * strlen("Alina") + 1);
apel3.prioritate = 3;
strcpy(apel3.numeApelant, "Alina");
apelUrgenta apel4;
apel4.numeApelant = (char*)malloc(sizeof(char) * strlen("Georgiana") + 1);
apel4.prioritate = 4;
strcpy(apel4.numeApelant, "Georgiana");
apelUrgenta apel5;
apel5.numeApelant = (char*)malloc(sizeof(char) * strlen("Georgiana") + 1);
apel5.prioritate = 5;
strcpy(apel5.numeApelant, "Georgiana");

heap h;
h = initializareHeap(0);
apelUrgenta rezApel;
inserareHeap(&h, apel1);
inserareHeap(&h, apel2);
inserareHeap(&h, apel3);
inserareHeap(&h, apel4);
inserareHeap(&h, apel5);
afisareHeap(h);
for (int i = h.dimensiune / 2 - 1; i >= 0; i--) {
	filtrare(h, i);
}
afisareHeap(h);
rezApel = extragereMaxim(&h);
printf("Prioritate %d\n", rezApel.prioritate);
rezApel = extragereMaxim(&h);
printf("Prioritate %d", rezApel.prioritate);
rezApel = extragereMaxim(&h);
printf("Prioritate %d", rezApel.prioritate);
rezApel = extragereMaxim(&h);
printf("Prioritate %d\n", rezApel.prioritate);
afisareHeap(h);

printf("\nBST-----------------\n");
BST* nodBST;
nodBST = creareNod(stud);
nodBST = inserareBST(nodBST, initializareStudent(2, "Alexandru"));
nodBST = inserareBST(nodBST, initializareStudent(3, "Marius"));
nodBST = inserareBST(nodBST, initializareStudent(4, "George"));
nodBST = inserareBST(nodBST, initializareStudent(5, "Maria"));
nodBST = inserareBST(nodBST, initializareStudent(6, "Alina"));
preordine(nodBST);
printf("\nAVL------------------\n");
BST* nodAVL;
nodAVL = creareNod(stud);
nodAVL = inserareBST(nodAVL, initializareStudent(2, "Alexandru"));
nodAVL = inserareBST(nodAVL, initializareStudent(3, "Marius"));
nodAVL = inserareBST(nodAVL, initializareStudent(4, "George"));
nodAVL = inserareBST(nodAVL, initializareStudent(5, "Maria"));
nodAVL = inserareBST(nodAVL, initializareStudent(6, "Alina"));
preordine(nodAVL);


}