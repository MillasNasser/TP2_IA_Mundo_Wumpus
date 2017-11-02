#include "ambiente.h"

void gera_mapa(){

}

int verifica_estado(int x, int y, ESTADO estado){
	//TO-DO: talvez converter X e Y pras coordenadas de matriz.
	return ((player.mundo_conhecido[x][y] & (estado)) != 0);
}
