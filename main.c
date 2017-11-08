#include "ambiente.h"

int main(){
    int i,j;
    inicializa_jogador();
    gera_mapa();

    imprime_mapa(mapa);

    /*while(!verica_estado(player.x, player.y, RELUSENTE)){
	    
	    SENTIDO temp;
	    agir(gera_acao(), temp);
    }*/
    return 0;
}
