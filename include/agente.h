#ifndef _AGENTE_H_
#define _AGENTE_H_

#include <stdio.h>
#include <stdlib.h>

#define TAM_MAPA 4
// define o pontuação caso o agente encontre o 
// ouro.
#define PREMIUM 10000
// define a penalidade sobre o agente caso 
// seja atirado uma flecha.
#define THROW_ARROW -10
// Macro para obter o módulo de uma operação.
#define MODULO(x) ((x)>=0?(x):-(x))

extern int verifica_estado();

extern char mapa[TAM_MAPA][TAM_MAPA];

typedef enum{
	ANDAR, ATIRAR, PEGAR
}ACAO;

typedef enum{
	NORTE, LESTE, SUL, OESTE
}SENTIDO;

typedef enum{
	BRISA=1, POCO=2, FEDOR=4, WUMPUS=8, RELUSENTE=16, CONHECIDO=32, VISITADO=64, JOGADOR=128
}ESTADO;

typedef struct s_Personagem{
	int x,y;
	int contador_movimento;
    int pontos;
	SENTIDO direcao;
	char mundo_conhecido[TAM_MAPA][TAM_MAPA];
}Personagem;

Personagem player;

/*TO-DO: Faz a movimentação do personagem
 *		 gastando pontos dependendo do movimento*/
void agir(ACAO acao, SENTIDO direcao);

/*Define a ação que o agente irá tomar*/
ACAO gera_acao();

/*TO-DO: Altera o atributo pontos do agente. 
 * (similar a um set).*/
void pontuar(int pontos);

/*TO-DO: (Altera as "coordenadas" x e y do agente)
 * realiza a movimentação do agente e aplica penalização.*/
void mover(SENTIDO sentido);

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
