#ifndef _AMBIENTE_H_
#define _AMBIENTE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "agente.h"

typedef enum{
	BRISA=1, POCO=2, FEDOR=4, WUMPUS=8, RELUSENTE=16
}ESTADO;

char mapa[TAM_MAPA][TAM_MAPA];

//Gerar o mapa aleatoriamente.
void gerar_mapa(char *nome_arquivo);

//Verificar se a posição escolhida está em volta da posição inicial do jogador. Se não estiver, retorna 1.
int verificar_redor_inicial_jogador(int jogador_x, int jogador_y, int posicao_x, int posicao_y);

//Criar o arquivo do mapa gerado aleatoriamente.
void criar_arquivo(char *nome_arquivo, char matriz[TAM_MAPA][TAM_MAPA]);

//Carrega o mapa do arquivo ou por parâmetro (foi gerado aleatoriamente). Verifica por erros no arquivo e inicia o mapa com todos os status corretos em cada posição.
void carregar_mapa(char *nome_arquivo, char matriz[TAM_MAPA][TAM_MAPA], int flag);

//Exibir o mapa na tela. (Debug)
void exibir_mapa();

//Inicia todas as posições do mapa com 0;
void iniciar_mapa();

//Inicia todas as posições do mapa temporário com ' - '.
void iniciar_mapa_arquivo(char matriz[TAM_MAPA][TAM_MAPA]);

//Aplica nas casas adjacentes a presença de um poço (brisa) ou wumpus (fedor).
void somar_rastros(char valor, int posicao_x, int posicao_y);

int verificar_mapa_valido();

//Verifica se as posições escolhidas para os poços possibilitam pelo menos uma solução para o jogador.
int verificar_solucao_possivel_mapa(int posicao_x, int posicao_y, char matriz[TAM_MAPA][TAM_MAPA]);

/*Retorna true se o estado é verdadeiro naquele ponto*/
int verica_estado(int x, int y, ESTADO estado); 

#endif /*_AMBIENTE_H_*/