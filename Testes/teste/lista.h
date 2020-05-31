#ifndef LISTA_H
#define LISTA_H

struct LISTA{
	struct LISTA *anterior;
	int valor;
	struct LISTA *proximo;
};
struct LISTA_CHAR{
	struct LISTA_CHAR *anterior;
	char valor;
	struct LISTA_CHAR *proximo;
};
struct topoLista{
	struct LISTA *inicio;
};
struct topoLista_CHAR{
	struct LISTA_CHAR *inicio;
	struct LISTA_CHAR *fim;
};
void show(struct LISTA *lista);
struct LISTA *alocar(int tamanho);
void empilha(int valorInput,struct topoLista *varTopo);
struct LISTA *topo(struct topoLista *varTopo);
struct LISTA *desempilha(struct topoLista *varTopo);
struct topoLista *initTopo(struct topoLista *p);
void finalizarLista(struct LISTA *lista);
void calcBin(int valor,int base,struct topoLista *topo);


struct LISTA_CHAR *alocar_CHAR(int tamanho);
void empilha_CHAR(char valorInput,struct topoLista_CHAR *varTopo);
struct LISTA_CHAR *desempilha_CHAR(struct topoLista_CHAR *varTopo);
void finalizarLista_CHAR(struct LISTA_CHAR *lista);
struct topoLista_CHAR *initTopo_CHAR(struct topoLista_CHAR *p);
void show_CHAR(struct LISTA_CHAR *lista);
int comparar_CHAR(struct LISTA_CHAR *lista1,struct LISTA_CHAR *lista2);
#endif