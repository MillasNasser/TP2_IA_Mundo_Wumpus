#include "ambiente.h"

void carregar_mapa(char *nome_arquivo, char matriz[TAM_MAPA][TAM_MAPA], int flag){
	iniciar_mapa();
	FILE *arquivo;
	//Flag = 1: O mapa é carregado do arquivo. Flag = 0: O mapa foi gerado aleatoriamente e passado por parâmetro.
	if(flag){
		arquivo=fopen(nome_arquivo,"r");
		if(arquivo == NULL){
			printf("Esse aquivo nao existe.");
			exit(0);
		}
	}
	int posicao_x,posicao_y;
	int jogador_iniciado=0, ouro_iniciado=0, wumpus_iniciado=0, quantidade_pocos=0;
	char estado;
	for(posicao_y=0;posicao_y<TAM_MAPA;posicao_y++){
		for(posicao_x=0;posicao_x<TAM_MAPA;posicao_x++){
			if(flag){
				fscanf(arquivo,"%c",&estado);
			}else{
				estado=matriz[posicao_y][posicao_x];
			}
			printf("%c",estado);
			switch(estado){
				case 'D':{
					if(!wumpus_iniciado){
						if(quantidade_pocos<3){
							mapa[posicao_y][posicao_x]|=WUMPUS+POCO;
							somar_rastros(FEDOR,posicao_y,posicao_x);
							somar_rastros(BRISA,posicao_y,posicao_x);
							wumpus_iniciado=1;
							quantidade_pocos++;
						}else{
							printf("O mapa só pode possuir ate 3 pocos.\n");
							exit(0);
						}
					}else{
						printf("O mapa só pode possuir um Wumpus.\n");
						exit(0);
					}
					break;
				}
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
					}else{
						printf("O mapa só pode possuir ate 3 pocos.\n");
						exit(0);
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
				if(flag)
					fscanf(arquivo," ");
				printf(" ");
			}
		}
		if(flag)
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
	if(flag)
		fclose(arquivo);
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
	if(posicao_x-1>=0)//Esquerda
		mapa[posicao_y][posicao_x-1]|=valor;

	if(posicao_y-1>=0)//Cima
		mapa[posicao_y-1][posicao_x]|=valor;
	
	if(posicao_x+1<TAM_MAPA)//Direita
		mapa[posicao_y][posicao_x+1]|=valor;
	
	if(posicao_y+1<TAM_MAPA)//Baixo
		mapa[posicao_y+1][posicao_x]|=valor;
}

void gerar_mapa(char *nome_arquivo){
	char mapa_arquivo[TAM_MAPA][TAM_MAPA];
	iniciar_mapa_arquivo(mapa_arquivo);

	//Escolhendo a posição inicial do jogador.
	int jogador_pos_x=TAM_MAPA-1,jogador_pos_y=0;
	mapa_arquivo[jogador_pos_x][jogador_pos_y]='J';

	//Gerando a posicao do ouro;
	int ouro_x,ouro_y;
	do{
		ouro_x=rand()%4;
		ouro_y=rand()%4;
	}while(mapa_arquivo[ouro_x][ouro_y]!='-' && verificar_redor_inicial_jogador(jogador_pos_x,jogador_pos_y,ouro_x,ouro_y));
	mapa_arquivo[ouro_x][ouro_y]='R';
	
	//Gerando as posições dos poços. Sempre serão gerados 3 poços.
	int pocos_validos=0;
	while(!pocos_validos){
		int posicoes[3][2];
		int valido=0;
		int i;
		for(i=0;i<3;i++){
			while(!valido){
				posicoes[i][0]=rand()%4;
				posicoes[i][1]=rand()%4;
				int pos0=posicoes[i][0], pos1=posicoes[i][1];
				if(mapa_arquivo[pos0][pos1]=='-' && verificar_redor_inicial_jogador(jogador_pos_x,jogador_pos_y,pos0,pos1)){
					mapa_arquivo[pos0][pos1]='P';
					valido=1;
				}
			}
			valido=0;
		}
		
		if(verificar_solucao_possivel_mapa(ouro_x,ouro_y,mapa_arquivo)){
			pocos_validos=1;
		}else{
			mapa_arquivo[posicoes[0][0]][posicoes[0][1]]='-';
			mapa_arquivo[posicoes[1][0]][posicoes[1][1]]='-';
			mapa_arquivo[posicoes[2][0]][posicoes[2][1]]='-';
		}
	}
	
	//Gerando posição do Wumpus.
	int wumpus_x,wumpus_y;
	do{
		wumpus_x=rand()%4;
		wumpus_y=rand()%4;
	}while(mapa_arquivo[wumpus_x][wumpus_y]!='-' && mapa_arquivo[wumpus_x][wumpus_y]!='P');
	if(mapa_arquivo[wumpus_x][wumpus_y]=='P'){
		mapa_arquivo[wumpus_x][wumpus_y]='D';
	}else{
		mapa_arquivo[wumpus_x][wumpus_y]='W';
	}

	criar_arquivo(nome_arquivo,mapa_arquivo);
	carregar_mapa(nome_arquivo,mapa_arquivo,0);
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
	
	if(posicao_x == TAM_MAPA-1 && posicao_y ==TAM_MAPA-1){//Posicao
		if(matriz[posicao_x-1][posicao_y]=='P' && matriz[posicao_x][posicao_y-1]=='P')
			return 0;
		if(matriz[posicao_x-1][posicao_y]=='P' && matriz[posicao_x-1][posicao_y-1]=='P' && matriz[posicao_x][posicao_y-2]=='P')
			return 0;
		if(matriz[posicao_x][posicao_y-1]=='P' && matriz[posicao_x-1][posicao_y-1]=='P' && matriz[posicao_x-2][posicao_y]=='P')
			return 0;
		return 1;
	}
	return 1;
}

int verificar_redor_inicial_jogador(int jogador_x, int jogador_y, int posicao_x, int posicao_y){
	if((posicao_x==TAM_MAPA-2 && posicao_y==0) || (posicao_x==TAM_MAPA-1 && posicao_y==1) || (posicao_x==TAM_MAPA-2 && posicao_y==1))
		return 0;	
	return 1;
}

void criar_arquivo(char *nome_arquivo, char matriz[TAM_MAPA][TAM_MAPA]){
	FILE *arquivo = fopen(nome_arquivo,"w");
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