#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

void show(struct LISTA *lista){
	/*
		Função Show Recebe o valor do topo da "LISTA" e recursivamente printa todos os valores até a base da pilha,
		Isso se deve ao ponteiro de próximo item estar NULL,
	*/
	if (lista->anterior==NULL)
	{
		printf("Estrutura: \n");
	}
	printf("%d\n", lista->valor);
	if (lista->proximo != NULL)
	{
		show(lista->proximo);
		printf("\n\n");
	}
};
void show_CHAR(struct LISTA_CHAR *lista){
	/*
		Função Show Recebe o valor do topo da "LISTA_CHAR" e recursivamente printa todos os valores até a base da pilha,
		Isso se deve ao ponteiro de próximo item estar NULL,
	*/
	if (lista->anterior==NULL)
	{
		printf("Invertido: \n");
	}
	printf("%c", lista->valor);
	if (lista->proximo != NULL)
	{
		show_CHAR(lista->proximo);
		printf("\n\n");
	}
};

struct LISTA *alocar(int tamanho){
	/*
		Retorna o espaço de memória do tamanho da struct "LISTA", basta passar como tamanho = 1;
	*/
	return ( (struct LISTA *) malloc(sizeof(struct LISTA) * tamanho) );
};
void empilha(int valorInput,struct topoLista *varTopo){
	/*
		Recebe o ponteiro que indica o topo da Lista e inserer o item no topo, tal qual ilustrado abaixo:
			|Topo|-> NULL
			|Topo|->|Item1|
			|Topo|->|Item2|<->|Item1|
			|Topo|->|Item3|<->|Item2|<->|Item1|

		As setas indicam o ponteiro de "anterior" e "proximo" da struct "LISTA"
	*/
	struct LISTA *temporaria =  alocar(1);
	temporaria->valor = valorInput;
	temporaria->proximo = NULL;
	temporaria->anterior=NULL;
	if (varTopo->inicio != NULL)
	{
		temporaria->proximo=alocar(1);
		temporaria->proximo=varTopo->inicio;
		varTopo->inicio->anterior = alocar(1);
		varTopo->inicio->anterior = temporaria;
	}else{
		varTopo->inicio = alocar(1);
	}
	varTopo->inicio = temporaria;
};
struct LISTA *topo(struct topoLista *varTopo){
	/*
		Ao receber ponteiro de topo, o mesmo retorna a struct "LISTA" no topo
	*/
	return varTopo->inicio;
};
struct LISTA *desempilha(struct topoLista *varTopo){
	/*
		Recebe o ponteiro que indica o topo da Lista e inserer o item no topo, tal qual ilustrado abaixo:
			|Topo|->|Item3|<->|Item2|<->|Item1|
			|Topo|->|Item2|<->|Item1|
			|Topo|->|Item1|
			|Topo|-> NULL
		As setas indicam o ponteiro de "anterior" e "proximo" da struct "LISTA"
		Retorna a struct "LISTA" removida
	*/
	struct LISTA *temporaria =  alocar(1);
	temporaria = varTopo->inicio;
	varTopo->inicio = varTopo->inicio->proximo;
	if(temporaria->proximo!=NULL){
		temporaria->proximo->anterior = NULL;
		temporaria->proximo = NULL;
		temporaria->anterior = NULL;
	}else{
		free(varTopo->inicio);
		varTopo->inicio=NULL;
	}
	return temporaria;
};
struct topoLista *initTopo(struct topoLista *p){
	// Retorna o espaço de memória da struct "topoLista"
	p = (struct topoLista *) malloc(sizeof(struct topoLista));
	p->inicio = NULL;
	return p;
};
void finalizarLista(struct LISTA *lista){
	// Libera a memória de todos os itens na pilha
	if (lista->proximo != NULL && lista != NULL)
	{
		finalizarLista(lista->proximo);
		free(lista);
	}
};
void calcBin(int valor,int base,struct topoLista *topo){
	// Faz o calculo de base, se 2 calcula binário, se 10 decimal ,se 16 Hexadecimal ...
	if (valor==0)
	{
			return;
	}else if (valor>0)
	{
		empilha(valor%base,topo);
		calcBin(((valor-(valor%base))/base),base,topo);
	}else{
		printf("\n >>> Valor: não é válido %d\n", valor);
	}
};
struct LISTA_CHAR *alocar_CHAR(int tamanho){
	// Retorna o espaço de memória da struct "LISTA_CHAR"
	return ( (struct LISTA_CHAR *) malloc(sizeof(struct LISTA_CHAR) * tamanho) );
};
void empilha_CHAR(char valorInput,struct topoLista_CHAR *varTopo){
	/*
		Recebe o ponteiro que indica o topo da Lista e inserer o item no topo, tal qual ilustrado abaixo:
			|Topo|-> NULL
			|Topo|->|Item1|
			|Topo|->|Item2|<->|Item1|
			|Topo|->|Item3|<->|Item2|<->|Item1|

		As setas indicam o ponteiro de "anterior" e "proximo" da struct topoLista_CHAR
	*/
	struct LISTA_CHAR *temporaria =  alocar_CHAR(1);
	temporaria->valor = valorInput;
	temporaria->proximo = NULL;
	temporaria->anterior=NULL;
	if (varTopo->inicio != NULL)
	{
		temporaria->proximo=alocar_CHAR(1);
		temporaria->proximo=varTopo->inicio;
		varTopo->inicio->anterior = alocar_CHAR(1);
		varTopo->inicio->anterior = temporaria;
	}else{
		varTopo->inicio = alocar_CHAR(1);
		varTopo->fim = alocar_CHAR(1);
		varTopo->fim = temporaria;
	}
	varTopo->inicio = temporaria;
};
struct LISTA_CHAR *desempilha_CHAR(struct topoLista_CHAR *varTopo){
	/*
		Recebe o ponteiro que indica o topo da Lista e inserer o item no topo, tal qual ilustrado abaixo:
			|Topo|->|Item3|<->|Item2|<->|Item1|
			|Topo|->|Item2|<->|Item1|
			|Topo|->|Item1|
			|Topo|-> NULL
		As setas indicam o ponteiro de "anterior" e "proximo" da struct LISTA_CHAR
		Retorna a struct "LISTA" removida
	*/
	struct LISTA_CHAR *temporaria =  alocar_CHAR(1);
	temporaria = varTopo->inicio;
	varTopo->inicio = varTopo->inicio->proximo;
	temporaria->proximo->anterior = NULL;
	temporaria->proximo = NULL;
	temporaria->anterior = NULL;
	return temporaria;
};
struct topoLista_CHAR *initTopo_CHAR(struct topoLista_CHAR *p){
	/*
		Ao receber ponteiro de topo, o mesmo retorna a struct "topoLista_CHAR" no topo
	*/
	p = (struct topoLista_CHAR *) malloc(sizeof(struct topoLista_CHAR));
	p->inicio = NULL;
	return p;
};
void finalizarLista_CHAR(struct LISTA_CHAR *lista){
	// Libera a memória de todos os itens na pilha
	if (lista->proximo != NULL)
	{
		finalizarLista_CHAR(lista->proximo);
		free(lista);
	}
};
int comparar_CHAR(struct LISTA_CHAR *lista1,struct LISTA_CHAR *lista2){
	// Varre 2 listas, uma do começo pro fim e outra do fim pro começo,
	// Se for um palindromo ele retorna 0 se não retorna 1
	if (lista1->valor == lista2->valor)
	{
		if (lista1->proximo == NULL && lista2->anterior == NULL)
		{
			return 0;
		}else if(lista1->proximo == NULL || lista2->anterior == NULL){
			return 1;
		}else{
			return comparar_CHAR(lista1->proximo,lista2->anterior);
		};
	}else{
		return 1;
	}
	return 1;
};