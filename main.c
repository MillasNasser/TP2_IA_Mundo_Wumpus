#include "ambiente.h"

int main(){
    int i,j;
    inicializa_jogador();
    gera_mapa();
    for(i = 0; i<4; i++){
        for(j = 0; j<4; j++){
            printf("%2d ", mapa[i][j]);
        }printf("\n");
    }

    imprime_mundo_conhecido();
    
    for(i = 0; i<4; i++){
        for(j = 0; j<4; j++){
            printf("%2d ", mapa[i][j]);
        }printf("\n");
    }
    /*while(!verica_estado(player.x, player.y, RELUSENTE)){
	    
	    SENTIDO temp;
	    agir(gera_acao(), temp);
    }*/
    return 0;
}
