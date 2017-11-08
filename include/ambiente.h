#ifndef _AMBIENTE_H_
#define _AMBIENTE_H_

#include <stdio.h>
#include <stdlib.h>

#include "agente.h"

/*typedef enum{
	BRISA=1, POCO=2, FEDOR=4, WUMPUS=8, RELUSENTE=16, JOGADOR=32
}ESTADO;*/

char mapa[TAM_MAPA][TAM_MAPA];

/*TO-DO: Já verificar se o mapa é válido*/
void gera_mapa();

//TO-DO 1: precisa saber em qual matriz olhar.
//TO-DO 2: ver se for um ou todos (OR ou AND) os estados da máscara.
/**
 * Verifica se estado está presente na matriz.
 * @param mapa global ou mundo conhecido do jogador
 * @param x linha da matriz
 * @param y coluna da matriz
 * @param estado máscara de bit correspondente ao(s) estado(s) desejado(s).
 * @return 1 se um dos estados da máscara estiver presente.
 */
int verifica_estado(char mapa[TAM_MAPA][TAM_MAPA], int x, int y, ESTADO estado);

void imprime_mapa(char mapa[TAM_MAPA][TAM_MAPA]);

#endif /*_AMBIENTE_H_*/
