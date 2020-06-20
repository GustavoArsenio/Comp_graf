                                              /*   UNIVERSIDADE FEDERAL DA GRANDE DOURADOS - UFGD
                                                   FACULDADES DE CIENCIAS EXATAS E TECNOLOGIA - FACET
                                                               ENGENHARIA DE COMPUTACAO
                                                                 COMPUTACAO GRAFICA

                                                            ALUNO: NICOLAS PIERIM PEREIRA                                         */



// BIBLIOTECAS

#include <stdio.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#define PI 3.141592653589

GLfloat roda_z=40.0; // CAMERAS DOS EIXOS
GLfloat roda_x=0.0; // CAMERAS DOS EIXOS
GLfloat roda_y=40.0; // CAMERAS DOS EIXOS
GLuint idTextura;

GLfloat antebraco=0.0, braco=0.0, cabeca=0.0,pernaesquerda = 70.0, pernadireita = 0.0,
joelhoesquerdo = -70.0, joelhodireito = 0.0, ladobracodireito = 0.0, ladobracoesquerdo = 0.0,
ladodireitoperna = 0.0, ladoesquerdoperna = 0.0, ladocorpo = 0.0; // VARIAVEIS PARA CONTROLAR A QUANTIDADE DE GRAUS NA ROTACAO

GLuint braco1, busto, cabeca1, perna; // VARIAVEIS PARA ALTERAR A MUDANCA DAS ROTACOES

 int tester =0, testaladopernae = 0, testaladopernad = 0, testaladocorpo = 0; // VARIAVEIS PARA TESTAR SE QUER MEXER PARA OS LADOS
 bool flag_perna=true;
 int valor_perna_direita=10,valor_perna_esquerda=-10;
static void boneco() // DEFINE AS PRIMITICAS PARA O DESNEHO DO BONECO
{
   braco1 = glGenLists(1); // ADICIONA NA LISTA A PRIMITIVA BRACO PARA QUE SUAS FILHAS NA PILHA SEJAM ATUALIZADAS AUTOMATICAMENTE

   glNewList(braco1, GL_COMPILE); // CRIA NA LISTA
   glScalef(0.09, 0.28, 0.09); // ESCALA CONFOME O NECESSARIO
   glutWireCube(2.0); // CHAMA A PRIMITIVA CUBO PARA DESENHAR JA ESCALADO
   glEndList(); // FECHA A LISTA


   busto = glGenLists(1); // CRIA NA LISTA
   glNewList(busto, GL_COMPILE);
   glScalef(0.35, 0.525, 0.2); // ESCALA CONFOME O NECESSARIO
   glutWireCube(2.0); // CHAMA A PRIMITIVA CUBO PARA DESENHAR JA ESCALADO
   glEndList(); // FECHA A LISTA

   perna = glGenLists(1); // CRIA NA LISTA
   glNewList(perna, GL_COMPILE);
   glScalef(0.09, 0.28, 0.09);// ESCALA CONFOME O NECESSARIO
   glutWireCube(2.0); // CHAMA A PRIMITIVA CUBO PARA DESENHAR JA ESCALADO
   glEndList(); // FECHA A LISTA

}

void init(void)
{
   boneco();
   glClearColor(0.68f, 0.85f, 0.9f, 1.0f); // Pinta a tela na cor azul claro
   glLineWidth(3.0); // define a largura de todas as figuras como 3
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // modo de desenho dos poligonos
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT); // APAGA A TELA E PINTA CONFORME A COR NO BUFFER
   glColor3f (0.0, 0.0, 0.0); // COR DO BONECO
   glLoadIdentity(); // CARREGA A IDENTIDADE
   glTranslatef(0.0,0.0,-2.0);

   // RODA A CAMERA NO EIXO X
   glRotatef(roda_x,1.0,0.0,0.0);
   // RODA A CAMERA NO EIXO Y
   glRotatef(roda_y,0.0,1.0,0.0);
   // RODA A CAMERA NO EIXO Z
   glRotatef(roda_z,0.0,0.0,1.0);


   glScalef(1,1,1); // ESCALANDO

   // INICIO DO DESENHO DO CORPO


   glPushMatrix();
   if(testaladocorpo == 1) glRotatef(ladocorpo, 0.0, 0.0, 1.0);
   glCallList(busto); // CHAMA A PRIMITIVA CUBO PARA DESENHAR O OBJETO CORPO
   glPopMatrix();

   // FIM DO CORPO

   // INICIO DO DESENHO DAS PERNAS

   glPushMatrix();
   glTranslatef(0.2, -0.85, 0.0); // POSICAO DA PERNA NO UNIVERSO

   glTranslatef(0.0, 0.28, 0.0); // TRANSLADAR PARA ORIGEM PARA ROTACIONAR NA SEQUENCIA
   if(testaladopernad == 1) glRotatef(ladodireitoperna, 0.0, 1.0, 1.0); // TRANSLADAR PARA ORIGEM PARA ROTACIONAR NA SEQUENCIA
   else glRotatef(pernadireita, 1.0, 0.0, 0.0); // ROTACIONAR O JOELHO PARA FRENTE CONFORME A VARIAVEL EH SOMADA
   testaladopernad = 0;
   glTranslatef(0.0, -0.28, 0.0);
   glPushMatrix();
   glCallList(perna); // CHAMA A PRIMITIVA CUBO PARA DESENHAR O OBJETO PERNA
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.0, -0.6, 0.0);

   glTranslatef(0.0, 0.28, 0.0);
   glRotatef(joelhodireito, 1.0, 0.0, 0.0);
   glTranslatef(0.0, -0.28, 0.0);
   glCallList(perna);
   glPopMatrix();
   glPopMatrix();


   glPushMatrix();
   glTranslatef(-0.2, -0.85, 0.0); // POSICAO DA PERNA NO UNIVERSO

   glTranslatef(0.0, 0.28, 0.0); // ROTACIONAR NA ORIGEM
   if(testaladopernae == 1) glRotatef(ladoesquerdoperna, 0.0, 1.0, 1.0); // CONDICAO PARA ROTACIONAR PARA OS LADOS A PERNA
   else glRotatef(pernaesquerda, 1.0, 0.0, 0.0); // ROTACIONAR A PERNA PARA FRENTE CONFORME A VARIAVEL EH SOMADA
   testaladopernae = 0; // VARIAVEL PARA TESTAR SOMENTE
   glTranslatef(0.0, -0.28, 0.0);
   glPushMatrix();
   glCallList(perna); // CHAMA PRIMITIVA CUBO PARA DESENHAR O OBJETO PERNA ESQUERDA
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.0, -0.6, 0.0); // POSICAO DO OBJETO JOELHO ESQUERDO  NO UNIVERSO

   glTranslatef(0.0, 0.28, 0.0); // TRANSLADAR PARA ORIGEM PARA ROTACIONAR NA SEQUENCIA
   glRotatef(joelhoesquerdo, 1.0, 0.0, 0.0); // ROTACIONAR O JOELHO PARA FRENTE CONFORME A VARIAVEL EH SOMADA
   glTranslatef(0.0, -0.28, 0.0);
   glCallList(perna); // CHAMA A PRIMITIVA CUBO PARA DESENHAR O OBJETO PERNA
   glPopMatrix();
   glPopMatrix();

   // FIM DO DESENHO PERNAS


   // INICIO DO DESENHO DA CABECA

   glPushMatrix();
   glTranslatef(0.0, 0.73, 0.0); // POSICAO NO UNIVERSO PARA DESENHAR
   glRotatef(cabeca, 0.0, 1.0, 0.0); // ROTACIONAR NO PROPRIO EIXO, (COM A FUNCAO DE OLHAR PARA OS LADOS)
   glutWireSphere(0.2,100,100); // DESENHA UMA ESFERA COM A FUNCAO DA CABECA DO BONECO
   glPopMatrix();

   // FIM DESENHAR CABECA


   // INICIO DO DESENHO DOS BRACOS
   glPushMatrix();
   glTranslatef(-0.45, 0.26, 0.0); // POSICAO DO BRACO NO UNIVERSO

   glTranslatef(0.0, 0.28, 0.0); //  TRANSLADA PARA ORIGEM PARA ROTACIONAR NA SEQUENCIA
   if (tester ==1) glRotatef(ladobracodireito, 0.0, 1.0, 1.0); // CONDICAO PARA ROTACIONAR PARA OS LADOS CONFORME A VARIAVEL EH SOMADA
   else glRotatef(pernadireita*0.5, 1.0, 0.0, 0.0); // ROTACIONAR PARA FRENTE CONFORME A VARIAVEL BRACO EH ALTERADA
   glTranslatef(0.0, -0.28, 0.0);
   glPushMatrix();
   glCallList(braco1); // CHAMA A PRIMITIVA PARA DESENHAR NO UNIVERSO
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.0, -0.59, 0.0); // POSICAO DO ANTEBRACO NO UNIVERSO

   glTranslatef(0.0, 0.28, 0.0); //  TRANSLADA PARA ORIGEM PARA ROTACIONAR NA SEQUENCIA
   glRotatef(antebraco, 1.0, 0.0, 0.0);
   glTranslatef(0.0, -0.28, 0.0);
   glCallList(braco1); // CHAMA A PRIMITIVA PARA DESENHAR NO UNIVERSO
   glPopMatrix();
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.45, 0.26, 0.0); // POSICAO DO BRACO NO UNIVERSO

   glTranslatef(0.0, 0.28, 0.0); //  TRANSLADA PARA ORIGEM PARA ROTACIONAR NA SEQUENCIA
   if (tester ==1) glRotatef(ladobracoesquerdo, 0.0, 1.0, 1.0); // CONDICAO PARA ROTACIONAR PARA OS LADOS CONFORME A VARIAVEL EH SOMADA
   else glRotatef(pernaesquerda*0.5, 1.0, .0, 0.0); // ROTACIONAR PARA FRENTE CONFORME A VARIAVEL BRACO EH ALTERADA
   glTranslatef(0.0, -0.28, 0.0);
   glPushMatrix();
   glCallList(braco1); // CHAMA A PRIMITIVA PARA DESENHAR NO UNIVERSO
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0.0, -0.59, 0.0);  // POSICAO DO ANTEBRACO NO UNIVERSO

   glTranslatef(0.0, 0.28, 0.0); //  TRANSLADA PARA ORIGEM PARA ROTACIONAR NA SEQUENCIA
   glRotatef(antebraco, 1.0, 0.0, 0.0);
   glTranslatef(0.0, -0.28, 0.0);
   glCallList(braco1); // CHAMA A PRIMITIVA PARA DESENHAR NO UNIVERSO
   glPopMatrix();
   glPopMatrix();


   glFlush (); // ATUALIZAR DESENHO
}

///////////////////////////////////////
////                                 //
////            TEXTURA              //
////                                 //
///////////////////////////////////////
//
//GLuint LoadTexture(const char * filename, int width, int height) {
//
//	GLuint texture;
//	unsigned char * data;
//
//	FILE * file;
//	file = fopen( filename, "rb" );
//	printf("%s",filename);
//	if ( file == NULL ) return 0;
//	data = (unsigned char *)malloc( width * height * 3 );
//	//int size = fseek(file,);
//	fread( data, width * height * 3, 1, file );
//	fclose( file );
//
//	for(int i = 0; i < width * height ; ++i) {
//		int index = i*3;
//		unsigned char B,R;
//		B = data[index];
//		R = data[index+2];
//
//		data[index] = R;
//		data[index+2] = B;
//	}
//
//	glGenTextures( 1, &texture );
//	glBindTexture( GL_TEXTURE_2D, texture );
//	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
//	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );
//
//
//	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
//	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
//	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
//	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
//	free( data );
//
//	return texture;
//}
//float angle;
//void Inicializa (void)
//{
//	glEnable(GL_COLOR_MATERIAL);
//	//Habilita o uso de ilumina��o
//	glEnable(GL_LIGHTING);
//	// Habilita a luz de n�mero 0
//	glEnable(GL_LIGHT0);
//	// Habilita o depth-buffering
//	glEnable(GL_DEPTH_TEST);
//
//	// Inicializa a vari�vel que especifica o �ngulo da proje��o
//	// perspectiva
//	angle=50;
//
//	// Inicializa as vari�veis usadas para alterar a posi��o do
//	// observador virtual
//
//
//	idTextura = LoadTexture("C:\\Users\\gustavo.silva\\Desktop\\Projeto\\tijolos.bmp",128,128);
//
//	glBindTexture(GL_TEXTURE_2D, idTextura);
//
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//	glEnable(GL_TEXTURE_2D);
//}


void reshape (int w, int h) // BIBLOTECA PADRAO DA OPENGL E GLUT, SEM MUITAS ALTERACOES AQUI
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glOrtho (-2.0, 2.0, -2.0, 2.0, 0.0, 5.0); // DEFINE AS COORDENADAS EM 3D
   glMatrixMode (GL_MODELVIEW);
}
void keyboard(unsigned char key, int x, int y) // FAZ O SCANNER DO TECLADO E DAS TECLAS PRESSIONADAS PELO USUARIO
{
//    if(key=='w'){
//        printf("\n Antes \n");
//                printf(" PernaDireita   %f\nJoelhoDireito   %f\n",pernadireita,joelhodireito);
//                printf(" PernaEsquerda   %f\nJoelhoEsquerda   %f\n",pernaesquerda,joelhoesquerdo);
//
//                joelhodireito = joelhodireito + 5.0;
//
//                int cont;
//                cont=0;
//                while(cont<3){
//
//                    cont++;
//                    pernadireita = pernadireita - 10.0;
//Sleep(1000);
//                    joelhodireito = joelhodireito + 12.0;
//Sleep(1000);
//                    joelhoesquerdo = joelhoesquerdo + 5.0;
//                }
//
//                Sleep(100);
//                joelhodireito = joelhodireito - 5.0;
//                Sleep(500);
//                glutMainLoop();
//
//             printf("\n Depois \n");
//             printf(" PernaDireita   %f\nJoelhoDireito   %f\n",pernadireita,joelhodireito);
//             printf(" PernaEsquerda   %f\nJoelhoEsquerda   %f\n",pernaesquerda,joelhoesquerdo);
//
//
//    }


  switch (key) { // VERIFICA TODAS AS POSSIBILIDADES DE CLIQUE E REALIZA A ACAO PEDIDA CONFORME O MANUAL NO CONSOLE

        case 'w':


        printf(" Pernadireita   %f\nJoelhodireito   %f\n",pernadireita,joelhodireito);
        if (pernadireita >= 60.0 & flag_perna){
            valor_perna_direita=-10;
        }

        if(pernadireita <= -10.0){
            valor_perna_direita=10;
        }
            pernadireita += valor_perna_direita;
            joelhodireito -= valor_perna_direita;
            pernaesquerda -= valor_perna_direita;
            joelhoesquerdo += valor_perna_direita;

        break;

        case 's':
            printf(" Pernadireita   %f\nJoelhodireito   %f\n",pernadireita,joelhodireito);
            if (pernadireita >= 60.0 & flag_perna){
                valor_perna_direita=10;
            }

            if(pernadireita <= -10.0){
                valor_perna_direita=-10;
            }
                pernadireita -= valor_perna_direita;
                joelhodireito += valor_perna_direita;
                pernaesquerda += valor_perna_direita;
                joelhoesquerdo -= valor_perna_direita;
        break;

        case 'r':

        for(int cont=0;cont<3;cont++){
        pernaesquerda = pernaesquerda - 10.0;
        if (pernaesquerda < -90.0) pernaesquerda = -90.0;

        joelhoesquerdo = joelhoesquerdo + 10.0;
        if (joelhoesquerdo < -50.0) joelhoesquerdo = -50.0;

        joelhodireito = joelhodireito + 5.0;
        if (joelhodireito < -50.0) joelhodireito = -50.0;
        }



        printf(" Pernadireita   %f\nJoelhodireito   %f\n",pernadireita,joelhodireito);
        break;

      case '2':
        antebraco = antebraco - 10.0;
        tester = 0;

        if (antebraco < -90.0) antebraco = -90.0;
        break;

      case '1':

        braco = braco - 10.0;
        tester =0;

        if (braco < -90.0) braco = -90.0;
        break;

      case '3':

        cabeca = cabeca + 10.0;
        break;

      case '4':

        pernaesquerda = pernaesquerda - 10.0;

        if (pernaesquerda < -90.0) pernaesquerda = -90.0;
        break;

      case '5':

        joelhoesquerdo = joelhoesquerdo + 10.0;

        if (joelhoesquerdo < -50.0) joelhoesquerdo = -50.0;
        break;

     case '6':

        pernadireita = pernadireita - 10.0;

        if (pernadireita < -90.0) pernadireita = -90.0;
        break;

      case '7':

        joelhodireito = joelhodireito - 10.0;

        if (joelhodireito < -50.0) joelhodireito = -50.0;
        break;

      case '8':

        ladobracodireito = ladobracodireito - 10.0;
        tester = 1;

        if (ladobracodireito < -50.0) ladobracodireito = -50.0;
        break;

      case '9':

        ladobracoesquerdo = ladobracoesquerdo + 10.0;
        tester = 1;

        if (ladobracoesquerdo > 50.0) ladobracoesquerdo = 50.0;
        break;

      case 'p':

        ladoesquerdoperna = ladoesquerdoperna - 10.0;
        testaladopernae = 1;

        if (ladoesquerdoperna < -50.0) ladoesquerdoperna = -50.0;

        break;

      case 't':

        ladodireitoperna = ladodireitoperna + 10.0;
        testaladopernad = 1;

        if (ladodireitoperna > 50.0) ladodireitoperna = 50.0;
        break;

      case 'x':

	     roda_x = roda_x + 10.0;
        break;

      case 'y':

	     roda_y = roda_y + 10.0;
        break;

      case 'z':

	     roda_z = roda_z + 10.0;
        break;

      case 27:
         exit(0);
         return;
   }
   glutPostRedisplay(); // CHAMA PARA ATUALIZAR O DESENHO
}



int main(int argc, char** argv)
{
    setlocale(LC_ALL, "Portuguese"); // SETA COMO PORTUGUES PARA ACENTO

    // MENU DE NAVEGACAO

    printf("MENU NAVEGAÇÃO\n");
    printf("TECLAS PARA NAVEGAÇÃO\n");
    printf("[x] [y] [z] - ALTERAM A VISÃO DO USUÁRIO CONFORME OS ÂNGULOS NO PLANO CARTESIANO\n");
    printf("[1] - MOVIMENTA OS BRAÇOS PARA FRENTE  \n");
    printf("[2] - MOVIMENTA OS ANTEBRAÇOS PARA FRENTE  \n");
    printf("[4] [6] - MOVIMENTAM AS PERNAS PARA FRENTE  \n");
    printf("[5] [7] - MOVIMENTAM OS JOELHOS PARA FRENTE  \n");
    printf("[8] [9] - MOVIMENTAM OS BRAÇOS PARA OS LADOS  \n");
    printf("[r] [t] - MOVIMENTAM AS PERNAS PARA OS LADOS  \n");

// COMANDOS PADROES DA OPENGL E GLUT
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
  // TAMANHO DA JANELA
   glutInitWindowSize (700, 700);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);

 //INICIA
   init ();

   glutDisplayFunc(display); // CHAMA O DESENHO
   glutReshapeFunc(reshape); // CHAMA PARA ATUALIZAR
   glutKeyboardFunc(keyboard); // CHAMA O TECLADO
//   Inicializa();
   glutMainLoop();
   return 0;
}
