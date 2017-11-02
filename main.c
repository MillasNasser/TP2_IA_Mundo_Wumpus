#include "ambiente.h"

int main(){
	inicializa_jogador();
	gera_mapa();
	while(!verica_estado(player.x, player.y, RELUSENTE)){
		agir(gera_acao());
	}
	return 0;
}
