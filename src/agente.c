#include "agente.h"

void inicializa_jogador(){
	
}

ACAO gera_acao(){
	return 0;
}

void agir(ACAO acao){

}

void __imprime_mundo_conhecido_linha(char hor, char vert, int x){
	int i;
	for(i = 0; i < (TAM_MAPA * x) + TAM_MAPA + 1; i++){
		printf("%c", (i % TAM_MAPA) ? hor : vert);
	}
	printf("\n");
}

void imprime_mundo_conhecido(){
	int i, j, k, l;
	int x = 3, y = 2;
	char vert = ':', hor = '-';
	int bit;

	char simbolos[7] = {' ', 'B', 'P', 'F', 'W', 'R', 'J'};
	
	__imprime_mundo_conhecido_linha(hor, vert, x);
	for(i=0; i<TAM_MAPA; i++){
		for(l=0; l<y; l++){
			printf("%c", vert);
			for(j=0; j<TAM_MAPA; j++){
				for(k=0; k<x; k++){
					bit = k + (l*x);
					printf("%c", simbolos[ (bit+1) * verifica_estado(i, j, 1 << bit) ]);
				}
				printf("%c", vert);
			}
			printf("\n");
		}
		__imprime_mundo_conhecido_linha(hor, vert, x);
	}
	printf("\n");
}
