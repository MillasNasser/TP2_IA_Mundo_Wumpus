#include "ambiente.h"

void gera_mapa(){
	mapa[3][0] = JOGADOR;	mapa[1][0] = WUMPUS;
	mapa[0][1] = RELUSENTE;	mapa[0][3] = POCO;
	mapa[2][2] = POCO;		mapa[3][2] = POCO;
	mapa[0][0] = FEDOR;		mapa[1][1] = FEDOR;
	mapa[2][0] = FEDOR;		mapa[0][2] = BRISA;
	mapa[1][2] = BRISA;		mapa[1][3] = BRISA;
	mapa[2][3] = BRISA;		mapa[3][3] = BRISA;
	mapa[2][1] = BRISA;		mapa[3][1] = BRISA;
}

int verifica_estado(char mapa[TAM_MAPA][TAM_MAPA], int x, int y, ESTADO estado){
	return ((mapa[x][y] & (estado)) == estado);
}

void __imprime_mapa_linha(char hor, char vert, int x){
	int i;
	for(i = 0; i < (TAM_MAPA * x) + TAM_MAPA + 1; i++){
		printf("%c", (i % TAM_MAPA) ? hor : vert);
	}
	printf("\n");
}

void imprime_mapa(char mapa[TAM_MAPA][TAM_MAPA]){
	int i, j, k, l;
	int x = 3, y = 2;
	char vert = ':', hor = '-';
	int bit;

	char simbolos[7] = {' ', 'B', 'F', 'P', 'W', 'R', 'J'};
	
	__imprime_mapa_linha(hor, vert, x);
	for(i=0; i<TAM_MAPA; i++){
		for(l=0; l<y; l++){
			printf("%c", vert);
			for(j=0; j<TAM_MAPA; j++){
				for(k=0; k<x; k++){
					bit = k + (l*x);
					printf("%c", simbolos[ (bit+1) * verifica_estado(mapa, i, j, 1 << bit) ]);
				}
				printf("%c", vert);
			}
			printf("\n");
		}
		__imprime_mapa_linha(hor, vert, x);
	}
	printf("\n");
}
