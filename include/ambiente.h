#ifndef _AMBIENTE_H_
#define _AMBIENTE_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TODOS_ESTADOS -1

typedef enum ESTADO{
	BRISA = 1,
	FEDOR = 2,
	POCO = 4,
	WUMPUS = 8,
	RELUSENTE = 16,
	JOGADOR = 32,
	CONHECIDO = 64,
	VISITADO = 128

} ESTADO;

#define TAM_MAPA 4

char mapa[TAM_MAPA][TAM_MAPA];

//Gerar o mapa aleatoriamente.
void gerar_mapa(char *nome_arquivo);

//Verificar se a posição escolhida está em volta da posição inicial do jogador. Se não estiver, retorna 1.
int verificar_redor_inicial_jogador(int linha, int coluna);

//Criar o arquivo do mapa gerado aleatoriamente.
void criar_arquivo(char *nome_arquivo, char matriz[TAM_MAPA][TAM_MAPA]);

//Carrega o mapa do arquivo ou por parâmetro (foi gerado aleatoriamente). Verifica por erros no arquivo e inicia o mapa com todos os status corretos em cada posição.
void carregar_mapa(char *nome_arquivo, char matriz[TAM_MAPA][TAM_MAPA], int flag);

//Inicia todas as posições do mapa com 0;
void iniciar_mapa();

//Inicia todas as posições do mapa temporário com ' - '.
void iniciar_mapa_arquivo(char matriz[TAM_MAPA][TAM_MAPA]);

//Verifica se as posições escolhidas para os poços possibilitam pelo menos uma solução para o jogador.
int verificar_solucao_possivel_mapa(int linha, int coluna, char matriz[TAM_MAPA][TAM_MAPA]);

/**
 * Verifica se estado está presente na matriz.
 * @param mapa global ou mundo conhecido do jogador
 * @param linha linha da matriz
 * @param coluna coluna da matriz
 * @param estado máscara de bit correspondente ao(s) estado(s) desejado(s).
 * @return 1 se um dos estados da máscara estiver presente.
 */
int verifica_estado(char mapa[TAM_MAPA][TAM_MAPA], int linha, int coluna, ESTADO estado);

void imprime_mapa(char mapa[TAM_MAPA][TAM_MAPA]);

void adicionar_estado(char mapa[TAM_MAPA][TAM_MAPA], int linha, int coluna, ESTADO estado);

void remover_estado(char mapa[TAM_MAPA][TAM_MAPA], int linha, int coluna, ESTADO estado);

void adicionar_estados_adjacentes(char mapa[TAM_MAPA][TAM_MAPA], int linha, int coluna, ESTADO estado);

void remover_estados_adjacentes(char mapa[TAM_MAPA][TAM_MAPA], int linha, int coluna, ESTADO estado);

#endif /*_AMBIENTE_H_*/
