#include "ambiente.h"

int main(int argc,char **argv){
	srand( (unsigned)time(NULL) );
	if(argv[1] != NULL){
		carregar_mapa(argv[1]);
	}else{
		gerar_mapa("teste_gerado.txt");
	}
	//inicializa_jogador();
	//gera_mapa();
	
	exit(0);
	while(!verica_estado(player.x, player.y, RELUSENTE)){
		agir(gera_acao());
	}
	return 0;
}
