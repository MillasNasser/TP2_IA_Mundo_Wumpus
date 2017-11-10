#include "agente.h"

int main(int argc,char **argv){
			
	srand( (unsigned)time(NULL) );
	if(argv[1] != NULL){
		
		char matriz[TAM_MAPA][TAM_MAPA];
		carregar_mapa(argv[1],matriz,1);
	}else{
		
		gerar_mapa("teste_gerado.txt");
	}		
	inicializa_jogador();
	imprime_mapa(mapa);
	exit(0);
	while(!verifica_estado(mapa, player.coluna, player.linha, RELUSENTE)){
			
		gera_acao(0);		
	}
	return 0;
}
