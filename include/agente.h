#ifndef _AGENTE_H_
#define _AGENTE_H_

#include "ambiente.h"


// define o pontuação caso o agente encontre o 
// ouro.
#define PREMIUM 10000
// define a penalidade sobre o agente caso 
// seja atirado uma flecha.
#define THROW_ARROW -10
// Macro para obter o módulo de uma operação.
#define MODULO(x) ((x)>=0?(x):-(x))

//a é no formato ab (a,b): ous seja representar 20
//é equivalente à (2,0)
#define vetorizar(a) (a/10)*TAM_MAPA + (a%10)
#define matri_x(v_vect) v_vect%TAM_MAPA 
#define matri_y(v_vect) v_vect/TAM_MAPA

typedef enum{
    
    ANDAR, ATIRAR, PEGAR
}ACAO;

typedef enum{
    
    NORTE, LESTE, SUL, OESTE
}SENTIDO;

typedef struct s_Personagem{
    
    int x,y;
    int contador_movimento;
    int pontos;
    int flecha;
    SENTIDO direcao;
    char mundo_conhecido[TAM_MAPA][TAM_MAPA];
}Personagem;

Personagem player;

extern int verifica_estado();

extern char mapa[TAM_MAPA][TAM_MAPA];

/*TO-DO: Faz a movimentação do personagem
 *		 gastando pontos dependendo do movimento*/
void agir(ACAO acao, SENTIDO direcao);

/*Define a ação que o agente irá tomar*/
ACAO gera_acao(int nivel);

int marcar_estados_adj();

void ag_retirar_estado(ESTADO estado);

void ag_adicionar_estado(ESTADO estado);

/*TO-DO: Altera o atributo pontos do agente. 
 * (similar a um set).*/
void pontuar(int pontos);

/*TO-DO: (Altera as "coordenadas" x e y do agente)
 * realiza a movimentação do agente e aplica penalização.*/
void andar(SENTIDO sentido);

void inicializa_jogador();

/*TO-DO: Altera a direção (NORTE, SUL, LESTE e OESTE)
 * do agente e aplica penalização.*/
void rotacionar(SENTIDO newSentido);

/*TO-DO: Aplica pontuação final para o agente e remove 
 * ouro do mapa. (FLAG PREMIUM).*/
void pegarOuro();

/*TO-DO: Remove o wumpus do mapa e aplica penalização
 * ao agente por atirar a flecha. (FLAG THROW_AXE)*/
void atirarFlecha();

/*TO-DO: Identifica qual ação deve ser tomada (ROTATE, 
 * ANDAR, ATIRAR, PEGAR) e chama a função respectiva.*/
void agir(ACAO acao, SENTIDO sentido);

/**
 * Imprime na tela um pequeno mapa com todas as informações que o jogador
 * conhece.
 */
void imprime_mundo_conhecido();

#endif /*_AGENTE_H_*/
