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

#define hash(linha, coluna) ((linha) * 10) + (coluna)

typedef enum ACAO{
	ANDAR, ATIRAR, PEGAR
} ACAO;

typedef enum SENTIDO{
	NORTE, LESTE, SUL, OESTE
} SENTIDO;

typedef struct Personagem{
	int linha;
	int coluna;
	int contador_movimento;
	int pontos;
	int flecha;
	SENTIDO direcao;
	char mundo_conhecido[TAM_MAPA][TAM_MAPA];
} Personagem;

Personagem player;

//extern int verifica_estado();

extern char mapa[TAM_MAPA][TAM_MAPA];

/*TO-DO: Faz a movimentação do personagem
 *		 gastando pontos dependendo do movimento*/
void agir(ACAO acao, SENTIDO direcao);

/*Define a ação que o agente irá tomar*/
int gera_acao(char pai[TAM_MAPA * TAM_MAPA], int ultimo);

int marcar_estados_adj();

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
void atirarFlecha(SENTIDO sentido);

/*TO-DO: Identifica qual ação deve ser tomada (ROTATE, 
 * ANDAR, ATIRAR, PEGAR) e chama a função respectiva.*/
void agir(ACAO acao, SENTIDO sentido);

/**
 * Imprime na tela um pequeno mapa com todas as informações que o jogador
 * conhece.
 */
void imprime_mundo_conhecido();

#endif /*_AGENTE_H_*/
