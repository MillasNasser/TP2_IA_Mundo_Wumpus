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

//char mapa[TAM_MAPA][TAM_MAPA];
char mapa[TAM_MAPA][TAM_MAPA];

/*TO-DO: Já verificar se o mapa é válido*/
void gerar_mapa();

int verificar_redor_inicial_jogador(int jogador_x, int jogador_y, int posicao_x, int posicao_y, char matriz[TAM_MAPA][TAM_MAPA]);

void criar_arquivo(char *nome_arquivo, char matriz[TAM_MAPA][TAM_MAPA]);

void carregar_mapa(char *nome_arquivo);

void exibir_mapa();

void iniciar_mapa();

void somar_rastros(char valor, int posicao_x, int posicao_y);

int verificar_mapa_valido();

int verificar_solucao_possivel_mapa(int posicao_x, int posicao_y, char matriz[TAM_MAPA][TAM_MAPA]);

/*Retorna true se o estado é verdadeiro naquele ponto*/
int verica_estado(int x, int y, ESTADO estado); 

#endif /*_AMBIENTE_H_*/