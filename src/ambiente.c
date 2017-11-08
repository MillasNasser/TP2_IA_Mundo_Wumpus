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

int verifica_estado(int x, int y, ESTADO estado){
	//TO-DO: talvez converter X e Y pras coordenadas de matriz.
	return ((player.mundo_conhecido[x][y] & (estado)) == estado);
}
