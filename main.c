#include "ambiente.h"

int main(){
	inicializa_jogador();
	gera_mapa();
	while(!verifica_estado(player.x, player.y, RELUSENTE)){
		agir(gera_acao());
		imprime_mundo_conhecido();
	}
	return 0;
}
