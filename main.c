#include "ambiente.h"

int main(){
    
    inicializa_jogador();
    gera_mapa();
    while(!verica_estado(player.x, player.y, RELUSENTE)){
	    
	    SENTIDO temp;
	    agir(gera_acao(), temp);
    }
    return 0;
}
