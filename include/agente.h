#ifndef _AGENTE_H_
#define _AGENTE_H_

#include "ambiente.h"

// define o pontuação caso o agente encontre o 
// ouro.
#define PREMIUM 1000
// define a penalidade sobre o agente caso 
// seja atirado uma flecha.
#define THROW_ARROW -10

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

void inicializa_jogador();

///Altera o atributo pontos do agente. (similar um set).
void pontuar(int pontos);

/**
 * Altera as "coordenadas" x e y do agente. Realiza a movimentação do agente e
 * aplica penalização.
 */
void andar(SENTIDO sentido);

///Altera a direção (NORTE, SUL, LESTE e OESTE) do agente e aplica penalização.
void rotacionar(SENTIDO newSentido);

///Aplica pontuação final para o agente e remove ouro do mapa. (FLAG PREMIUM).
void pegarOuro();

/**
 * Remove o wumpus do mapa e aplica penalização ao agente por atirar a flecha.
 * (FLAG THROW_AXE)
 */
void atirarFlecha(SENTIDO sentido);

/**
 * Identifica qual ação deve ser tomada (ROTATE, ANDAR, ATIRAR, PEGAR) e chama a
 * função respectiva.*/
void agir(ACAO acao, SENTIDO sentido);

///Utiliza o mundo conhecido para tentar deduzir o que existe no mapa.
int marcar_estados_adj();

///Define a ação que o agente irá tomar
int gera_acao(char pai[TAM_MAPA * TAM_MAPA], int ultimo);

/**
 * Imprime na tela um pequeno mapa com todas as informações que o jogador
 * conhece.
 */
void imprime_mundo_conhecido();

/**
 * Exibe uma mensagem na tela e encerra o jogo mostrando a pontuação final do
 * agente.
 */
void finaliza(const char *mensagem);

#endif /*_AGENTE_H_*/
