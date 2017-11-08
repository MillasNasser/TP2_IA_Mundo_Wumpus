#include "agente.h"

void inicializa_jogador(int posicao_y, int posicao_x){
	player.x = posicao_y;
	player.y = posicao_x;
	int i,j;
	for(i=0;i<TAM_MAPA;i++){
		for(j=0;j<TAM_MAPA;j++){
			player.mundo_conhecido[i][j]=0;
		}
	}
	
}

ACAO gera_acao(){
	return 0;
}

void agir(ACAO acao){

}

void imprime_mundo_conhecido(){
	
}