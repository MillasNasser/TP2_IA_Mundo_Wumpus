#include "ambiente.h"

void carregar_mapa(char *nome_arquivo){
	iniciar_mapa();
	FILE *arquivo=fopen(nome_arquivo,"r");
	if(arquivo == NULL){
		printf("Esse aquivo nao existe.");
		exit(0);
	}
	int tamanho_mapa,posicao_x,posicao_y;
	int jogador_iniciado=0, ouro_iniciado=0, wumpus_iniciado=0, quantidade_pocos=0;
	char estado;
	fscanf(arquivo,"%d\n",&tamanho_mapa);
	for(posicao_y=0;posicao_y<tamanho_mapa;posicao_y++){
		for(posicao_x=0;posicao_x<tamanho_mapa;posicao_x++){
			fscanf(arquivo,"%c",&estado);
			printf("%c",estado);
			switch(estado){
				case 'W':{
					if(!wumpus_iniciado){
						
						mapa[posicao_y][posicao_x]|=WUMPUS;
						somar_rastros(FEDOR,posicao_y,posicao_x);
						wumpus_iniciado=1;
					}else{
						printf("O mapa só pode possuir um Wumpus.\n");
						exit(0);
					}
					break;
				}
				case 'P':{
					if(quantidade_pocos<3){
						mapa[posicao_y][posicao_x]|=POCO;
						somar_rastros(BRISA,posicao_y,posicao_x);
						quantidade_pocos++;
					}
					break;
				}
				case 'R':{
					if(!ouro_iniciado){
						mapa[posicao_y][posicao_x]|=RELUSENTE;
						ouro_iniciado=1;
					}else{
						printf("Erro: O mapa só pode possuir uma barra de ouro.\n");
						exit(0);
					}
					break;
				}
				case 'J':{
					if(!jogador_iniciado){
						if((posicao_y == 0 || posicao_y == TAM_MAPA-1) && (posicao_x == 0 || posicao_x == TAM_MAPA-1)){
							inicializa_jogador(posicao_y,posicao_x);
							jogador_iniciado=1;
						}else{
							printf("Erro: O jogador só pode iniciar nas posicoes dos extremos do mapa.\n");
							exit(0);
						}
					}else{
						printf("Erro: O jogador só pode iniciar em uma posicao, apenas.\n");
						exit(0);
					}
					break;
				}
				case '-':{
					break;
				}
				default:{
					printf("\nCaracter invalido encontrado no arquivo:'%c'.\n",estado);
					exit(0);
				}
			}
			if(posicao_x+1<TAM_MAPA){
				fscanf(arquivo," ");
				printf(" ");
			}
		}
		fscanf(arquivo,"\n");
		printf("\n");
	}
	if(!jogador_iniciado){
		printf("\nO jogador não foi iniciado no mapa!\n");
		exit(0);
	}
	if(!ouro_iniciado){
		printf("\nO mapa nao possui nenhum ouro!\n");
		exit(0);
	}
	
	printf("\n\n");
	exibir_mapa();
	fclose(arquivo);
	
	//Backtracking
	if(!verificar_mapa_valido()){
		printf("O mapa gerado do arquivo nao possui solucao!\n");
		exit(0);
	}
}

void exibir_mapa(){
	int i,j;
	printf("\n");
	for(i=0;i<TAM_MAPA;i++){
		for(j=0;j<TAM_MAPA;j++){
			printf("%d ",mapa[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
}

void iniciar_mapa(){
	int i,j;
	for(i=0;i<TAM_MAPA;i++){
		for(j=0;j<TAM_MAPA;j++){
			mapa[i][j]=0;
		}
	}
}

void iniciar_mapa_arquivo(char matriz[TAM_MAPA][TAM_MAPA]){
	int i,j;
	for(i=0;i<TAM_MAPA;i++){
		for(j=0;j<TAM_MAPA;j++){
			matriz[i][j]='-';
		}
	}
}

void somar_rastros(char valor, int posicao_y, int posicao_x){
	//Esquerda
	printf("\nteste: [%d/%d]\n",posicao_y-1,posicao_x);
	if(posicao_x-1>=0){
		//printf("\nValor:%d\n",mapa[posicao_x][posicao_y-1]);
		mapa[posicao_y][posicao_x-1]|=valor;
		
	}
	
	//Cima
	
	if(posicao_y-1>=0){
		mapa[posicao_y-1][posicao_x]|=valor;
	}
	
	//Direita
	if(posicao_x+1<TAM_MAPA){
		mapa[posicao_y][posicao_x+1]|=valor;
	}
	
	//Baixo
	if(posicao_y+1<TAM_MAPA){
		mapa[posicao_y+1][posicao_x]|=valor;
	}
}

void gerar_mapa(char *nome_arquivo){
	char mapa_arquivo[TAM_MAPA][TAM_MAPA];
	//do{
		iniciar_mapa_arquivo(mapa_arquivo);
		
		//Escolhendo a posição inicial do jogador.
		//int posicao_jogador=rand()%4;
		int jogador_pos_x,jogador_pos_y;
		switch(rand()%4){ //O fator de divisão é 4 por causa dos 4 cantos do mapa, não do tamanho do mapa.
			case 0:{
				jogador_pos_x=0;
				jogador_pos_y=0;
				break;
			}
			case 1:{ 
				jogador_pos_x=0;
				jogador_pos_y=TAM_MAPA-1;
				break;
			}
			case 2:{
				jogador_pos_x=TAM_MAPA-1;
				jogador_pos_y=0;
				break;
			}
			case 3:{
				jogador_pos_x=TAM_MAPA-1;
				jogador_pos_y=TAM_MAPA-1;
			}
			
		}
		mapa_arquivo[jogador_pos_x][jogador_pos_y]='J';
		
		//Gerando a posicao do ouro;
		int ouro_x,ouro_y;
		do{
			ouro_x=rand()%4;
			ouro_y=rand()%4;
		}while(mapa_arquivo[ouro_x][ouro_y]!='-');
		mapa_arquivo[ouro_x][ouro_y]='R';
		
		//Gerando as posições dos poços. Sempre serão gerados 3 poços.
		int pocos_validos=0;
		while(!pocos_validos){
			int posicoes[3][2];
			int valido=0;
			int i,j;
			for(i=0;i<3;i++){
				while(!valido){
					for(j=0;j<2;j++){
						posicoes[i][j]=rand()%4;
					}
					int pos0=posicoes[i][0], pos1=posicoes[i][1];
					if(mapa_arquivo[pos0][pos1]=='-' && !verificar_redor_inicial_jogador(jogador_pos_x,jogador_pos_y,pos0,pos1,mapa_arquivo)){
						mapa_arquivo[pos0][pos1]='P';
						valido=1;
					}
				}
			}
			if(verificar_solucao_possivel_mapa(ouro_x,ouro_y,mapa_arquivo)){
				pocos_validos=1;
			}else{
				mapa_arquivo[posicoes[0][0]][posicoes[0][1]]='-';
				mapa_arquivo[posicoes[1][0]][posicoes[1][1]]='-';
				mapa_arquivo[posicoes[2][0]][posicoes[2][1]]='-';
			}
			
		}
		
		int wumpus_x,wumpus_y;
		do{
			wumpus_x=rand()%4;
			wumpus_y=rand()%4;
		}while(mapa_arquivo[wumpus_x][wumpus_y]!='-');
		mapa_arquivo[wumpus_x][wumpus_y]='W';
		
		criar_arquivo(nome_arquivo,mapa_arquivo);
		carregar_mapa(nome_arquivo);
	//}while(!verificar_solucao_possivel_mapa());
}

int verificar_mapa_valido(){
	
	return 1;
}

int verificar_solucao_possivel_mapa(int posicao_x, int posicao_y, char matriz[TAM_MAPA][TAM_MAPA]){
	//Verifica o redor do ouro para descobrir se é possível que o jogador possa pegá-lo.
	if(posicao_x==0 && (posicao_y>0 && posicao_y<TAM_MAPA-1)){//Linha 0
		if(matriz[posicao_x][posicao_y-1] =='P' && matriz[posicao_x+1][posicao_y]=='P' && matriz[posicao_x][posicao_y+1] == 'P')
			return 0;
		return 1;
	}
	
	if(posicao_x==TAM_MAPA-1 && (posicao_y>0 && posicao_y<TAM_MAPA-1)){//Linha TAM_MAPA-1
		if(matriz[posicao_x][posicao_y-1] =='P' && matriz[posicao_x-1][posicao_y]=='P' && matriz[posicao_x][posicao_y+1] == 'P')
			return 0;
		return 1;
	}
	
	if((posicao_x>0 && posicao_x<TAM_MAPA-1) && posicao_y==0){//Coluna 0
		if(matriz[posicao_x-1][posicao_y] =='P' && matriz[posicao_x][posicao_y+1]=='P' && matriz[posicao_x+1][posicao_y] == 'P')
			return 0;
		return 1;
	}
	
	if((posicao_x>0 && posicao_x<TAM_MAPA-1) && posicao_y==TAM_MAPA-1){//Coluna TAM_MAPA-1
		if(matriz[posicao_x-1][posicao_y] =='P' && matriz[posicao_x][posicao_y-1]=='P' && matriz[posicao_x+1][posicao_y] == 'P')
			return 0;
		return 1;
	}
	
	if(posicao_x == 0 && posicao_y ==0){//Posicao [0,0]
		if(matriz[posicao_x+1][posicao_y]=='P' && matriz[posicao_x][posicao_y+1]=='P')
			return 0;
		if(matriz[posicao_x+1][posicao_y]=='P' && matriz[posicao_x+1][posicao_y+1]=='P' && matriz[posicao_x][posicao_y+2]=='P')
			return 0;
		if(matriz[posicao_x][posicao_y+1]=='P' && matriz[posicao_x+1][posicao_y+1]=='P' && matriz[posicao_x+2][posicao_y]=='P')
			return 0;
		return 1;
	}
	
	if(posicao_x == 0 && posicao_y ==TAM_MAPA-1){//Posicao [0,TAM_MAPA-1]
		if(matriz[posicao_x+1][posicao_y]=='P' && matriz[posicao_x][posicao_y-1]=='P')
			return 0;
		if(matriz[posicao_x+1][posicao_y]=='P' && matriz[posicao_x+1][posicao_y-1]=='P' && matriz[posicao_x][posicao_y-2]=='P')
			return 0;
		if(matriz[posicao_x][posicao_y-1]=='P' && matriz[posicao_x+1][posicao_y-1]=='P' && matriz[posicao_x+2][posicao_y]=='P')
			return 0;
		return 1;
	}
	
	if(posicao_x == TAM_MAPA-1 && posicao_y ==0){//Posicao [TAM_MAPA-1,0]
		if(matriz[posicao_x-1][posicao_y]=='P' && matriz[posicao_x][posicao_y+1]=='P')
			return 0;
		if(matriz[posicao_x-1][posicao_y]=='P' && matriz[posicao_x-1][posicao_y+1]=='P' && matriz[posicao_x][posicao_y+2]=='P')
			return 0;
		if(matriz[posicao_x][posicao_y+1]=='P' && matriz[posicao_x-1][posicao_y+1]=='P' && matriz[posicao_x-2][posicao_y]=='P')
			return 0;
		return 1;
	}
	
	if(posicao_x == TAM_MAPA-1 && posicao_y ==TAM_MAPA-1){//Posicao
		if(matriz[posicao_x-1][posicao_y]=='P' && matriz[posicao_x][posicao_y-1]=='P'){
			return 0;
		}
		if(matriz[posicao_x-1][posicao_y]=='P' && matriz[posicao_x-1][posicao_y-1]=='P' && matriz[posicao_x][posicao_y-2]=='P'){
			return 0;
		}
		if(matriz[posicao_x][posicao_y-1]=='P' && matriz[posicao_x-1][posicao_y-1]=='P' && matriz[posicao_x-2][posicao_y]=='P'){
			return 0;
		}
		return 1;
	}
	return 0;
}

int verificar_redor_inicial_jogador(int jogador_x, int jogador_y, int posicao_x, int posicao_y, char matriz[TAM_MAPA][TAM_MAPA]){
	if(jogador_x == 0 && jogador_y ==0){
		if((posicao_x==1 && posicao_y==0) || (posicao_x==0 && posicao_y==1) || (posicao_x==1 && posicao_y==1))
			return 0;
		return 1;
	}
	
	if(jogador_x == 0 && jogador_y ==TAM_MAPA-1){
		if((posicao_x==0 && posicao_y==TAM_MAPA-2) || (posicao_x==1 && posicao_y==TAM_MAPA-1) || (posicao_x==1 && posicao_y==TAM_MAPA-2))
			return 0;
		return 1;
	}
	
	if(jogador_x == TAM_MAPA-1 && jogador_y ==0){
		if((posicao_x==TAM_MAPA-2 && posicao_y==0) || (posicao_x==TAM_MAPA-1 && posicao_y==1) || (posicao_x==TAM_MAPA-2 && posicao_y==1))
			return 0;
		return 1;
	}
	
	if(jogador_x == TAM_MAPA-1 && jogador_y ==TAM_MAPA-1){
		if((posicao_x==TAM_MAPA-1 && posicao_y==TAM_MAPA-2) || (posicao_x==TAM_MAPA-2 && posicao_y==TAM_MAPA-1) || (posicao_x==TAM_MAPA-2 && posicao_y==TAM_MAPA-2))
			return 0;
		return 1;
	}
	return 0;
}

void criar_arquivo(char *nome_arquivo, char matriz[TAM_MAPA][TAM_MAPA]){
	FILE *arquivo = fopen(nome_arquivo,"w");
	fprintf(arquivo,"%d\n",TAM_MAPA);
	int i,j;
	for(i=0;i<TAM_MAPA;i++){
		for(j=0;j<TAM_MAPA;j++){
			fprintf(arquivo,"%c",matriz[i][j]);
			if(j<TAM_MAPA-1){
				fprintf(arquivo," ");
			}
		}
		fprintf(arquivo,"\n");
	}
	fclose(arquivo);
}

int verica_estado(int x, int y, ESTADO estado){
	return 0;
}