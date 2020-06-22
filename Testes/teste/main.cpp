//http://www.lighthouse3d.com/tutorials/glut-tutorial/keyboard-example-moving-around-the-world/



/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */
#include <windows.h>
#include <math.h>
#include <time.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

static int slices = 16;
static int stacks = 16;
GLuint idTextura;

// Mouse Configs

float deltaAngle = 0.0f;
float deltaAngle2 = 0.0f;
float deltaMove = 0;
int xOrigin = -1;
int yOrigin = -1;
float angle2 = 0.0f;
float y_global= 5.00f;
float x_global= 422.65f;
float z = 188.35f;
float switch_maps=1;

//minimap Configs

 int subWindow2;
 int mainWindow;
 int subWindow3;
 int subWindow1;
 int border = 6;
 int xMiniMap;
 int yMiniMap;
 int zMiniMap;

  // width and height of the window

 int h,w;


//---------------- Estruturas para colisão ----------------//
struct LISTA{
	struct LISTA *anterior;
	float x,y,z;
	struct LISTA *proximo;
};

struct topoLista{
	struct LISTA *inicio;
};

struct topoLista * topo_lista;

//---------------- Metodos para monipulação das estruturas de colisão ----------------//
int isIn(float x,float y, float z,struct LISTA *cubo,float size_column){
    /*
        isIn é uma função que valida se o x,y informado está dentro do cubo
    */


    int isInX=0,isInY=0,isInZ=0;
    // Definir em X
    isInX = (cubo->x+3+size_column/2 >= x && x >= cubo->x-(3+size_column/2));

    // Definir em Y
    isInY = (cubo->z+3+size_column/2 >= y && y >= cubo->z-(3+size_column/2));

    // Definir em z
    isInZ = (cubo->y+3+size_column/2 >= z && z >= cubo->y-(3+size_column/2));

    return (isInX == 1 & 1 == isInY & isInZ ==1);
};
int hasColided(float x,float z,float y,struct LISTA *cubo,float size_column){
    if ( cubo->proximo == NULL ){
        return 0;
    }else if( isIn( x, z, y, cubo, size_column ) == 1){
        printf("\n#=======================================#\n");
        printf("# EM X: %.2f,%.2f,%.2f\n", cubo->x,x,cubo->x);
        printf("# EM Z: %.2f,%.2f,%.2f\n", cubo->z,z,cubo->z);
        printf("# EM Y: %.2f,%.2f,%.2f\n", cubo->y,y,cubo->y);
        printf("#=======================================#\n");

        return 1;
    }else{
        return hasColided( x, z, y, cubo->proximo, size_column);
    }
}


void show(struct LISTA *lista){
	/*
		Função Show Recebe o valor do topo da "LISTA" e recursivamente printa todos os valores até a base da pilha,
		Isso se deve ao ponteiro de próximo item estar NULL,
	*/
	if (lista->anterior==NULL)
	{
		printf("Estrutura: \n");
	}
	/*
	printf("#=======================================#\n");
	printf("# x1:%.2f,y1:%.2f,z1:%.2f\n", lista->x1,lista->y1,lista->z1);
	printf("# x2:%.2f,y2:%.2f,z2:%.2f\n", lista->x2,lista->y2,lista->z2);
	printf("#=======================================#\n");
	*/
	if (lista->proximo != NULL)
	{
		show(lista->proximo);
		printf("\n\n");
	}
};
struct LISTA *alocar(int tamanho){
	/*
		Retorna o espaço de memória do tamanho da struct "LISTA", basta passar como tamanho = 1;
	*/
	return ( (struct LISTA *) malloc(sizeof(struct LISTA) * tamanho) );
};
int comparar(struct LISTA *lista_original,struct LISTA *lista_de_parametros){
    /*
    if((lista_original->x1<=lista_original->x1<=lista_original->x2) && (lista_original->y1<=lista_original->y1<=lista_original->y2))
        return 1;
    else
    */
        return 0;

};

void empilha(float x,float y,float z,struct topoLista *varTopo){
	/*
		Recebe o ponteiro que indica o topo da Lista e inserer o item no topo, tal qual ilustrado abaixo:
			|Topo|-> NULL
			|Topo|->|Item1|
			|Topo|->|Item2|<->|Item1|
			|Topo|->|Item3|<->|Item2|<->|Item1|
		As setas indicam o ponteiro de "anterior" e "proximo" da struct "LISTA"
	*/
	struct LISTA *temporaria =  alocar(1);
	temporaria->x = x;
    temporaria->y = y;
    temporaria->z = z;
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

// -------------------------- FIM BLOCO DE ESTUTURAS PARA COLISAO --------------------------//


float mod (float num){
    if (num < 0){
            return num * -1;
    }else{
            return num;
    };
};

/////////////////////////////////////
//                                 //
//              MAPA               //
//                                 //
/////////////////////////////////////

static int raw_map_x=40,raw_map_z=120;

static int raw_map[40][120]={

{0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,1,0,0,1,1,0,0,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
{0,1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0},
{0,1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,0,0,1,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,1,0,0,0,1,1,0,0,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,1,1,0,0,0,1,0,0,1,0,0,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,0,0,1,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,1,1,1,0,0,1,0,0,1,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,1,1,1,0,0,0,0,0,1,0,0,0,1,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,0,0,1,0,0,1,0,0,1,1,1,1,1,1,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,1,1,1,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,1,0,1,0,0,0,0,1,1,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,1,1,0,0,0,1,1,1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,0,0,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,1,1,0,0,0,0},
{0,1,0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,1,1,0,0,0,0},
{0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,1,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,1,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,0,0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
{0,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,1,0,0,0,0},
{0,1,0,0,1,1,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,0,1,1,0,0,0,0},
{0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
{0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
    };
// Definição para tamanho de cada bloco no mapa
static float size_column = 5.0f;
/* GLUT callback Handlers */
// angle of rotation for the camera direction
float angle=0.0f,angle_y=0.0f;
// actual vector representing the camera's direction
float lx=0.64f,lz=-0.76f,ly=0.0f;
// XZ position of the camera


/////////////////////////////////////
//                                 //
//            TEXTURA              //
//                                 //
/////////////////////////////////////
// GHS FUNC
GLuint LoadTexture(const char * filename, int width, int height) {

	GLuint texture;
	unsigned char * data;

	FILE * file;
	file = fopen( filename, "rb" );
	if ( file == NULL ) return 0;
	data = (unsigned char *)malloc( width * height * 3 );
	//int size = fseek(file,);
	fread( data, width * height * 3, 1, file );
	fclose( file );

	for(int i = 0; i < width * height ; ++i) {
		int index = i*3;
		unsigned char B,R;
		B = data[index];
		R = data[index+2];

		data[index] = R;
		data[index+2] = B;
	}

	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );


	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
	free( data );

	return texture;
}

// GHS FUNC






void Inicializa (void)
{

	// Inicializa a vari�vel que especifica o �ngulo da proje��o
	// perspectiva
	angle=50;

	// Inicializa as vari�veis usadas para alterar a posi��o do
	// observador virtual

    idTextura = LoadTexture("C:\\Users\\gustavo.silva.DESKTOP-62LR2B8\\Desktop\\teste\\tijolos.bmp",128,128);

  	glBindTexture(GL_TEXTURE_2D, idTextura);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glEnable(GL_TEXTURE_2D);
}


// GHS FUNC adptaçao
void drawCube() {
    glBegin ( GL_QUADS );
		// Face frontal
	    glTexCoord2f(0.0f         , 0.0f         ); glVertex3f(-1 * size_column/2, -1 * size_column/2,  size_column/2     );
		glTexCoord2f(size_column/2, 0.0f         ); glVertex3f(     size_column/2, -1 * size_column/2,  size_column/2     );
		glTexCoord2f(size_column/2, size_column/2); glVertex3f(     size_column/2,      size_column/2,  size_column/2     );
		glTexCoord2f(0.0f         , size_column/2); glVertex3f(-1 * size_column/2,      size_column/2,  size_column/2     );
		// Face posterior
		glTexCoord2f(size_column/2, 0.0f         ); glVertex3f(-1 * size_column/2, -1 * size_column/2, -1 * size_column/2 );
		glTexCoord2f(size_column/2, size_column/2); glVertex3f(-1 * size_column/2,      size_column/2, -1 * size_column/2 );
		glTexCoord2f(0.0f         , size_column/2); glVertex3f(     size_column/2,      size_column/2, -1 * size_column/2 );
		glTexCoord2f(0.0f         , 0.0f         ); glVertex3f(     size_column/2, -1 * size_column/2, -1 * size_column/2 );
		// Face superior
		glTexCoord2f(0.0f         , size_column/2); glVertex3f(-1 * size_column/2,  size_column/2, -1 * size_column/2     );
		glTexCoord2f(0.0f         , 0.0f         ); glVertex3f(-1 * size_column/2,  size_column/2,      size_column/2     );
		glTexCoord2f(size_column/2, 0.0f         ); glVertex3f(     size_column/2,  size_column/2,      size_column/2     );
		glTexCoord2f(size_column/2, size_column/2); glVertex3f(     size_column/2,  size_column/2, -1 * size_column/2     );
		// Face inferior
		glTexCoord2f(size_column/2, size_column/2); glVertex3f(-1 * size_column/2, -1 * size_column/2, -1 * size_column/2 );
		glTexCoord2f(0.0f         , size_column/2); glVertex3f(     size_column/2, -1 * size_column/2, -1 * size_column/2 );
		glTexCoord2f(0.0f         , 0.0f         ); glVertex3f(     size_column/2, -1 * size_column/2,      size_column/2 );
		glTexCoord2f(size_column/2, 0.0f         ); glVertex3f(-1 * size_column/2, -1 * size_column/2,      size_column/2 );
		// Face lateral direita
		glTexCoord2f(size_column/2, 0.0f         ); glVertex3f(     size_column/2, -1 * size_column/2, -1 * size_column/2 );
		glTexCoord2f(size_column/2, size_column/2); glVertex3f(     size_column/2,      size_column/2, -1 * size_column/2 );
		glTexCoord2f(0.0f         , size_column/2); glVertex3f(     size_column/2,      size_column/2,      size_column/2 );
		glTexCoord2f(0.0f         , 0.0f         ); glVertex3f(     size_column/2, -1 * size_column/2,      size_column/2 );
		// Face lateral esquerda
		glTexCoord2f(0.0f         , 0.0f         ); glVertex3f(-1 * size_column/2, -1 * size_column/2, -1 * size_column/2 );
		glTexCoord2f(size_column/2, 0.0f         ); glVertex3f(-1 * size_column/2, -1 * size_column/2,      size_column/2 );
		glTexCoord2f(size_column/2, size_column/2); glVertex3f(-1 * size_column/2,      size_column/2,      size_column/2 );
		glTexCoord2f(0.0f         , size_column/2); glVertex3f(-1 * size_column/2,      size_column/2, -1 * size_column/2 );
	glEnd();

}
//-------------------------- VISUALIZAR BLOCOS PARA COLISAO --------------------------//
void draw_map_by_colision(struct LISTA * cubo){
    struct LISTA * new_cubo = cubo;
    int position = 1;
    do{
                    glPushMatrix();
                    glTranslatef(new_cubo->x,new_cubo->y,new_cubo->z);
                    glColor3f(1.0f, 1.0f, 1.0f);
                    glutSolidCube(size_column);
                    glPopMatrix();
                    new_cubo = new_cubo->proximo;
    }while(new_cubo != NULL);
	glEnd();
}

void drawTeto() {

        for(int i = 0; i < raw_map_x; i++)
            for(int j = 0; j < raw_map_z; j++) {
                                       //Constroi  Teto
              glPushMatrix();
                    glTranslatef(j * size_column,size_column*2+(size_column/3),i * size_column);
                    drawCube();
                    glPopMatrix();

            }
}

void drawMap() {
    if(switch_maps == 1){
        for(int i = 0; i < raw_map_x; i++)
            for(int j = 0; j < raw_map_z; j++) {
                if(raw_map[i][j] == 1){
                    glPushMatrix();
                    glTranslatef(j * size_column,size_column/3,i * size_column);
                    drawCube();
                    glPopMatrix();
                }
                if(raw_map[i][j] == 1){
                    glPushMatrix();
                    glTranslatef(j * size_column,size_column+(size_column/3),i * size_column);
                    drawCube();
                    glPopMatrix();

                }
                             //Constroi  Teto



            }
    }else{
        draw_map_by_colision(topo_lista->inicio);
    }



}




/////////////////////////////////////
//                                 //
//            TECLADO              //
//                                 //
/////////////////////////////////////

static void key(unsigned char key, int x, int y)
{

float fraction = 0.1f;
//      //Posições
printf("\n===========================\n         POSICOES TECLADO\ny_global= %f\nx_global= %f\nz = %f\n",y_global,x_global,z);
printf("\n***************************\n         CAMERA TECLADO\nly= %f\nlx = %f\nlz = %f\n",ly,lx,lz);
	switch (key) {// Função Teclas
		case 'q' ://Camera para Esquerda
			angle -= 0.05f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case 'e' ://Camera para Direita
			angle += 0.05f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case 'w' ://Move a camera para frente
			x += lx;
			z += lz;
			x_global += lx;
			break;
		case 's' ://Move a camera para tras
			x -= lx;
			z -= lz;
			x_global -= lx;
			break;

			break;
            case 'h': deltaAngle2+=0.1f;
            ly = sin(angle2 + deltaAngle2);
            break;
            case 'b': deltaAngle2 -= 0.1f;
             ly = sin(angle2 + deltaAngle2);
             break;

        case ' '://Move a camera para cima
            y_global += 1.0f;
            break;

        case 'f'://Move a camera para baixo
             if (y_global >= 2.0f)
                 y_global -= 1.0f;
            break;

        case 'i':// seta para inicio mapa
y_global= 5.00f;
x_global= 422.65f;
z = 188.35f;
            break;

        case 'o':// seta para Fim mapa
y_global= 8.00f;
x_global= 299.88f;
z = 9.21f;
            break;
        case 'p':// seta para ponto "aleatorio" do mapa
            y_global = 20.0f;
            x_global = 55.55f;
            z = 435.08f;
            break;

        case 'x': //fecha o jogo
           exit(0);
            break;
        case 'k':
            if (switch_maps == 0){
                switch_maps = 1;
            }else{
                switch_maps = 0;
            }
            printf("\n >>> switch_maps: %.2f ",switch_maps);
            break;

	}
	if(hasColided(x_global,z,y_global,topo_lista->inicio,size_column) == 1){
        switch (key) {// Função Teclas

		case 'w' ://Move a camera para frente
			x -= lx;
			z -= lz;
			x_global -= lx;
			break;
		case 's' ://Move a camera para tras
			x += lx;
			z += lz;
			x_global += lx;
			break;
        case ' '://Move a camera para cima
            y_global -= 1.0f;
            break;

        case 'f'://Move a camera para baixo
                 y_global += 1.0f;
            break;
        }
    }
    glutPostRedisplay();
}

/////////////////////////////////////
//                                 //
//             MOUSE               //
//                                 //
/////////////////////////////////////
void mouseMove(int x, int y) {


		deltaAngle = (x - xOrigin) * 0.01f;// sen
		deltaAngle2=(y-yOrigin)* 0.001f;
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
		ly = -sin(angle2 + deltaAngle2);



//void mouseButton(int button, int state, int x, int y) {
//
//	if (button == GLUT_LEFT_BUTTON) {
//
//		if (state == GLUT_UP) {
//			angle += deltaAngle;
//			xOrigin = -1;
//			yOrigin = -1;
//			angle2 += deltaAngle2;
//		}
//		else {
//			xOrigin = x;
//			yOrigin = y;
//		}
//	}
//}
}
static void idle(void)
{
    glutPostRedisplay();
}


//MINIMAP

  void init() {

            glClearColor(0.0, 0.0, 0.0, 0.0);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);




	// Inicializa a vari�vel que especifica o �ngulo da proje��o
	// perspectiva
	angle=50;

	// Inicializa as vari�veis usadas para alterar a posi��o do
	// observador virtual

    idTextura = LoadTexture("D:\\projetos_git\\Comp_graf\\Testes\\teste\\tijolos.bmp",128,128);

  	glBindTexture(GL_TEXTURE_2D, idTextura);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glEnable(GL_TEXTURE_2D);
        }


   void renderScene() {
            glutSetWindow(mainWindow);
            glClear(GL_COLOR_BUFFER_BIT);
            glutSwapBuffers();
            init();
        }


 // Display func for sub window 2

        void renderScenesw1 (){

      glutSetWindow(subWindow2);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glLoadIdentity();
                        gluLookAt(	x_global, y_global, z,
                        x_global, y_global,  z,
                        lx, 0,  lz);

            // Draw red cone at the location of the main camera
            glPushMatrix();
            glColor3f(0.9f,0.9f,0.9f);
            glTranslatef(xMiniMap,yMiniMap,zMiniMap);
            glRotatef(180-(angle+deltaAngle)*180.0/3.14,0.0,1.0,0.0);
            glutSolidCone(0.2,0.8f,4,4);
            glPopMatrix();
             glBegin(GL_QUADS);
                glVertex3f( -1000, 0, -1000);
                glVertex3f( -1000, 0,  1000);
                glVertex3f(  1000, 0,  1000);
                glVertex3f(  1000, 0, -1000);
            glEnd();

            drawMap();
            glutSwapBuffers();


        }

        void renderScenesw2() {

            glutSetWindow(subWindow2);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glLoadIdentity();
                        gluLookAt(	x_global, y_global+15, z,
                        x_global, y_global,  z,
                        lx, 0,  lz);

            // Draw red cone at the location of the main camera
            glPushMatrix();
            glColor3f(0.9f,0.9f,0.9f);
            glTranslatef(xMiniMap,yMiniMap,zMiniMap);
            glRotatef(180-(angle+deltaAngle)*180.0/3.14,0.0,1.0,0.0);
            glutSolidCone(0.2,0.8f,4,4);
            glPopMatrix();
             glBegin(GL_QUADS);
                glVertex3f( -1000, 0, -1000);
                glVertex3f( -1000, 0,  1000);
                glVertex3f(  1000, 0,  1000);
                glVertex3f(  1000, 0, -1000);
            glEnd();

            drawMap();
            glutSwapBuffers();
        }

        void renderScenesw3() {

            glutSetWindow(subWindow3);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glLoadIdentity();
            gluLookAt(xMiniMap-lz*10 , yMiniMap, zMiniMap+lx*10,
                  xMiniMap ,yMiniMap ,zMiniMap ,
                  0.9f,0.9f,0.9f);

            // Draw red cone at the location of the main camera
            glPushMatrix();
            glColor3f(1.0,0.0,0.0);
            glTranslatef(xMiniMap,yMiniMap,zMiniMap);
            glRotatef(180-(angle+deltaAngle)*180.0/3.14,0.0,1.0,0.0);
            glutSolidCone(0.2,0.8f,4,4);
            glPopMatrix();

            drawMap();

            glutSwapBuffers();
        }

        // Global render func
        void renderSceneAll() {

            // check for keyboard movement
            //if (deltaMove) {
               // computePos(deltaMove);
              //  glutSetWindow(mainWindow);
              //  glutPostRedisplay();
        //    }







            renderScene();
            renderScenesw1();
            renderScenesw2();

           // renderScenesw3();
        }

             void setProjection(int w1, int h1)
        {
            float ratio;
            // Prevent a divide by zero, when window is too short
            // (you cant make a window of zero width).
            ratio = 1.0f * w1 / h1;
            // Reset the coordinate system before modifying
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            // Set the viewport to be the entire window
                glViewport(0, 0, w1, h1);

            // Set the clipping volume
            gluPerspective(45,ratio,0.1,1000);
            glMatrixMode(GL_MODELVIEW);
        }



// ILUMINAÇÃO
const GLfloat light_ambient[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 1.0f };

static void resize(int width, int height)
{

    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;


            // set subwindow 2 as the active window
            glutSetWindow(subWindow2);
            // resize and reposition the sub window
            glutPositionWindow((width+border)/1.335,border);
            glutReshapeWindow(width/4-border*3/2, height/4 - border*3/2);
            setProjection(width/2-border*3/2,height/2 - border*3/2);

}


static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(	x_global, y_global, z,
                x_global+lx, y_global+ly,  z+lz,
                0.0f, y_global,  0.0f);

        // Draw ground
	glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
        glVertex3f( -1000, 0, -1000);
        glVertex3f( -1000, 0,  1000);
        glVertex3f(  1000, 0,  1000);
        glVertex3f(  1000, 0, -1000);
    glEnd();
    drawMap();
    init();
    glutSwapBuffers();
    renderScenesw2();

    	drawTeto();


}

int main(int argc, char *argv[])
{
 {


    topo_lista = initTopo(topo_lista);
        for(int i = 0; i < raw_map_x; i++)
		for(int j = 0; j < raw_map_z; j++) {
            if(raw_map[i][j] == 1){
                empilha(
                            j * size_column, size_column/2, i * size_column,
                            topo_lista
                        );
            }

        }
        //show(topo_lista->inicio);



    //
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1000,700);
	mainWindow = glutCreateWindow("Labrinto");
    //glutFullScreen();

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
	glutPassiveMotionFunc(mouseMove);

	  // sub windows
    subWindow1 = glutCreateSubWindow(mainWindow, border,border,w-2*border, h/2 - border*3/2);
    glutDisplayFunc(renderScenesw1);
    init();


    subWindow2 = glutCreateSubWindow(mainWindow, border,(h+border)/2,w/2-border*3/2, h/2 - border*3/2);
    glutDisplayFunc(renderScenesw2);
    init();



    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

}
    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    init();

    glutMainLoop();
    return EXIT_SUCCESS;

}
