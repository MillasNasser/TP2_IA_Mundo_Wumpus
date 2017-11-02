#ifndef _AMBIENTE_H_
#define _AMBIENTE_H_

#include <stdio.h>
#include <stdlib.h>
#include "agente.h"

typedef enum{
	BRISA=1, POCO=2, FEDOR=4, WUMPUS=8, RELUSENTE=16, JOGADOR=32
}ESTADO;

char mapa[TAM_MAPA][TAM_MAPA];

/*TO-DO: Já verificar se o mapa é válido*/
void gera_mapa();

/*Retorna true se o estado é verdadeiro naquele ponto*/
int verifica_estado(int x, int y, ESTADO estado);

#endif /*_AMBIENTE_H_*/
