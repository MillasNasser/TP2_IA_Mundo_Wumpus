#include "agente.h"

int main(int argc,char **argv){
			
	srand( (unsigned)time(NULL) );
	if(argc > 1){
		
		char matriz[TAM_MAPA][TAM_MAPA];
		carregar_mapa(argv[1],matriz,1);
	}else{
		
		gerar_mapa("teste_gerado.txt");
	}		
	inicializa_jogador();
	imprime_mapa(mapa);
	//exit(0);
	char pai[TAM_MAPA * TAM_MAPA];
	int i;
	for(i=0; i<TAM_MAPA * TAM_MAPA; i++){
		pai[i] = -1;
	}
	gera_acao(pai, 1);
	return 0;
}
