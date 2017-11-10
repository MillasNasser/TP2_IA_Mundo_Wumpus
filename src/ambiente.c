#include "ambiente.h"

void carregar_mapa(char *nome_arquivo, char matriz[TAM_MAPA][TAM_MAPA], int flag){
	iniciar_mapa();
	FILE *arquivo;
	//Flag = 1: O mapa é carregado do arquivo. Flag = 0: O mapa foi gerado aleatoriamente e passado por parâmetro.
	if(flag){
		arquivo = fopen(nome_arquivo, "r");
		if(arquivo == NULL){
			printf("Esse aquivo nao existe.");
			exit(0);
		}
	}
	int posicao_x, posicao_y;
	int ouro_iniciado = 0, wumpus_iniciado = 0, quantidade_pocos = 0;
	char estado;
	for(posicao_y = 0; posicao_y < TAM_MAPA; posicao_y++){
		for(posicao_x = 0; posicao_x < TAM_MAPA; posicao_x++){
			if(flag){
				fscanf(arquivo, "%c", &estado);
			}else{
				estado = matriz[posicao_y][posicao_x];
			}
			printf("%c", estado);
			switch(estado){
				case 'D':
				{
					if(!wumpus_iniciado){
						if(quantidade_pocos < 3){
							mapa[posicao_y][posicao_x] |= WUMPUS + POCO;
							somar_rastros(FEDOR, posicao_y, posicao_x);
							somar_rastros(BRISA, posicao_y, posicao_x);
							wumpus_iniciado = 1;
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
				case 'W':
				{
					if(!wumpus_iniciado){
						mapa[posicao_y][posicao_x] |= WUMPUS;
						somar_rastros(FEDOR, posicao_y, posicao_x);
						wumpus_iniciado = 1;
					}else{
						printf("O mapa só pode possuir um Wumpus.\n");
						exit(0);
					}
					break;
				}
				case 'P':
				{
					if(quantidade_pocos < 3){
						mapa[posicao_y][posicao_x] |= POCO;
						somar_rastros(BRISA, posicao_y, posicao_x);
						quantidade_pocos++;
					}else{
						printf("O mapa só pode possuir ate 3 pocos.\n");
						exit(0);
					}
					break;
				}
				case 'R':
				{
					if(!ouro_iniciado){
						mapa[posicao_y][posicao_x] |= RELUSENTE;
						ouro_iniciado = 1;
					}else{
						printf("Erro: O mapa só pode possuir uma barra de ouro.\n");
						exit(0);
					}
					break;
				}
				case 'J':
				{
					/*if(!jogador_iniciado){
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
					}*/
					break;
				}
				case '-':
				{
					break;
				}
				default:
				{
					printf("\nCaracter invalido encontrado no arquivo:'%c'.\n", estado);
					exit(0);
				}
			}
			if(posicao_x + 1 < TAM_MAPA){
				if(flag)
					fscanf(arquivo, " ");
				printf(" ");
			}
		}
		if(flag)
			fscanf(arquivo, "\n");
		printf("\n");
	}
	if(!ouro_iniciado){
		printf("\nO mapa nao possui nenhum ouro!\n");
		exit(0);
	}
	printf("\n\n");
	if(flag)
		fclose(arquivo);
}

int verifica_estado(char mapa[TAM_MAPA][TAM_MAPA], int linha, int coluna, ESTADO estado){
	if(coluna >= 0 && coluna < 4 && linha >= 0 && linha < 4){
		return((mapa[linha][coluna] & (estado)) == estado);
	}
	return 0;
}

void iniciar_mapa(){
	int i, j;
	for(i = 0; i < TAM_MAPA; i++){
		for(j = 0; j < TAM_MAPA; j++){
			mapa[i][j] = 0;
		}
	}
}

void iniciar_mapa_arquivo(char matriz[TAM_MAPA][TAM_MAPA]){
	int i, j;
	for(i = 0; i < TAM_MAPA; i++){
		for(j = 0; j < TAM_MAPA; j++){
			matriz[i][j] = '-';
		}
	}
}

void somar_rastros(char valor, int linha, int coluna){
	if(coluna - 1 >= 0)//Esquerda
		mapa[linha][coluna - 1] |= valor;

	if(linha - 1 >= 0)//Cima
		mapa[linha - 1][coluna] |= valor;

	if(coluna + 1 < TAM_MAPA)//Direita
		mapa[linha][coluna + 1] |= valor;

	if(linha + 1 < TAM_MAPA)//Baixo
		mapa[linha + 1][coluna] |= valor;
}

void gerar_mapa(char *nome_arquivo){
	char mapa_arquivo[TAM_MAPA][TAM_MAPA];
	iniciar_mapa_arquivo(mapa_arquivo);

	//Escolhendo a posição inicial do jogador.
	int jogador_pos_x = TAM_MAPA - 1, jogador_pos_y = 0;
	mapa_arquivo[jogador_pos_x][jogador_pos_y] = 'J';

	//Gerando a posicao do ouro;
	int ouro_x, ouro_y;
	do{
		ouro_x = rand() % 4;
		ouro_y = rand() % 4;
	}while(mapa_arquivo[ouro_x][ouro_y] != '-' && verificar_redor_inicial_jogador(ouro_x, ouro_y));
	mapa_arquivo[ouro_x][ouro_y] = 'R';

	//Gerando as posições dos poços. Sempre serão gerados 3 poços.
	int pocos_validos = 0;
	while(!pocos_validos){
		int posicoes[3][2];
		int valido = 0;
		int i;
		for(i = 0; i < 3; i++){
			while(!valido){
				posicoes[i][0] = rand() % 4;
				posicoes[i][1] = rand() % 4;
				int pos0 = posicoes[i][0], pos1 = posicoes[i][1];
				if(mapa_arquivo[pos0][pos1] == '-' && verificar_redor_inicial_jogador(pos0, pos1)){
					mapa_arquivo[pos0][pos1] = 'P';
					valido = 1;
				}
			}
			valido = 0;
		}

		if(verificar_solucao_possivel_mapa(ouro_x, ouro_y, mapa_arquivo)){
			pocos_validos = 1;
		}else{
			mapa_arquivo[posicoes[0][0]][posicoes[0][1]] = '-';
			mapa_arquivo[posicoes[1][0]][posicoes[1][1]] = '-';
			mapa_arquivo[posicoes[2][0]][posicoes[2][1]] = '-';
		}
	}

	//Gerando posição do Wumpus.
	int wumpus_x, wumpus_y;
	do{
		wumpus_x = rand() % 4;
		wumpus_y = rand() % 4;
	}while(mapa_arquivo[wumpus_x][wumpus_y] != '-' && mapa_arquivo[wumpus_x][wumpus_y] != 'P');
	if(mapa_arquivo[wumpus_x][wumpus_y] == 'P'){
		mapa_arquivo[wumpus_x][wumpus_y] = 'D';
	}else{
		mapa_arquivo[wumpus_x][wumpus_y] = 'W';
	}

	criar_arquivo(nome_arquivo, mapa_arquivo);
	carregar_mapa(nome_arquivo, mapa_arquivo, 0);
}

int verificar_mapa_valido(){

	return 1;
}

int verificar_solucao_possivel_mapa(int linha, int coluna, char matriz[TAM_MAPA][TAM_MAPA]){
	//Verifica o redor do ouro para descobrir se é possível que o jogador possa pegá-lo.
	if(linha == 0 && (coluna > 0 && coluna < TAM_MAPA - 1)){//Linha 0
		if(matriz[linha][coluna - 1] == 'P' && matriz[linha + 1][coluna] == 'P' && matriz[linha][coluna + 1] == 'P')
			return 0;
		return 1;
	}

	if(linha == TAM_MAPA - 1 && (coluna > 0 && coluna < TAM_MAPA - 1)){//Linha TAM_MAPA-1
		if(matriz[linha][coluna - 1] == 'P' && matriz[linha - 1][coluna] == 'P' && matriz[linha][coluna + 1] == 'P')
			return 0;
		return 1;
	}

	if((linha > 0 && linha < TAM_MAPA - 1) && coluna == 0){//Coluna 0
		if(matriz[linha - 1][coluna] == 'P' && matriz[linha][coluna + 1] == 'P' && matriz[linha + 1][coluna] == 'P')
			return 0;
		return 1;
	}

	if((linha > 0 && linha < TAM_MAPA - 1) && coluna == TAM_MAPA - 1){//Coluna TAM_MAPA-1
		if(matriz[linha - 1][coluna] == 'P' && matriz[linha][coluna - 1] == 'P' && matriz[linha + 1][coluna] == 'P')
			return 0;
		return 1;
	}

	if(linha == 0 && coluna == 0){//Posicao [0,0]
		if(matriz[linha + 1][coluna] == 'P' && matriz[linha][coluna + 1] == 'P')
			return 0;
		if(matriz[linha + 1][coluna] == 'P' && matriz[linha + 1][coluna + 1] == 'P' && matriz[linha][coluna + 2] == 'P')
			return 0;
		if(matriz[linha][coluna + 1] == 'P' && matriz[linha + 1][coluna + 1] == 'P' && matriz[linha + 2][coluna] == 'P')
			return 0;
		return 1;
	}

	if(linha == 0 && coluna == TAM_MAPA - 1){//Posicao [0,TAM_MAPA-1]
		if(matriz[linha + 1][coluna] == 'P' && matriz[linha][coluna - 1] == 'P')
			return 0;
		if(matriz[linha + 1][coluna] == 'P' && matriz[linha + 1][coluna - 1] == 'P' && matriz[linha][coluna - 2] == 'P')
			return 0;
		if(matriz[linha][coluna - 1] == 'P' && matriz[linha + 1][coluna - 1] == 'P' && matriz[linha + 2][coluna] == 'P')
			return 0;
		return 1;
	}

	if(linha == TAM_MAPA - 1 && coluna == TAM_MAPA - 1){//Posicao
		if(matriz[linha - 1][coluna] == 'P' && matriz[linha][coluna - 1] == 'P')
			return 0;
		if(matriz[linha - 1][coluna] == 'P' && matriz[linha - 1][coluna - 1] == 'P' && matriz[linha][coluna - 2] == 'P')
			return 0;
		if(matriz[linha][coluna - 1] == 'P' && matriz[linha - 1][coluna - 1] == 'P' && matriz[linha - 2][coluna] == 'P')
			return 0;
		return 1;
	}
	return 1;
}

int verificar_redor_inicial_jogador(int linha, int coluna){
	if((linha == TAM_MAPA - 2 && coluna == 0) || (linha == TAM_MAPA - 1 && coluna == 1) || (linha == TAM_MAPA - 2 && coluna == 1))
		return 0;
	return 1;
}

void criar_arquivo(char *nome_arquivo, char matriz[TAM_MAPA][TAM_MAPA]){
	FILE *arquivo = fopen(nome_arquivo, "w");
	int i, j;
	for(i = 0; i < TAM_MAPA; i++){
		for(j = 0; j < TAM_MAPA; j++){
			fprintf(arquivo, "%c", matriz[i][j]);
			if(j < TAM_MAPA - 1){
				fprintf(arquivo, " ");
			}
		}
		fprintf(arquivo, "\n");
	}
	fclose(arquivo);
}

void __imprime_mapa_linha(char hor, char vert, int x){
	int i;
	for(i = 0; i < (TAM_MAPA * x) + TAM_MAPA + 1; i++){
		printf("%c", (i % TAM_MAPA) ? hor : vert);
	}
	printf("\n");
}

void imprime_mapa(char mapa[TAM_MAPA][TAM_MAPA]){
	int i, j, k, l;
	int x = 3, y = 2;
	char vert = ':', hor = '-';
	int bit;

	char simbolos[7][10] = {" ", "\033[1;34mB", "\033[1;35mF", "\033[0;34mP", "\033[1;31mW", "\033[01;33mR", "\033[1;32mJ"};

	char c = 0;
	
	__imprime_mapa_linha(hor, vert, x);
	for(i = 0; i < TAM_MAPA; i++){
		for(l = 0; l < y; l++){
			printf("%c", vert);
			for(j = 0; j < TAM_MAPA; j++){
				c = mapa[i][j];
				for(k = 0; k < x; k++){
					bit = k + (l * x);
					printf("%s\033[0m", simbolos[ (bit + 1) * verifica_estado(mapa, i, j, 1 << bit) ]);
				}
				printf("%c", vert);
			}
			printf("\n");
		}
		__imprime_mapa_linha(hor, vert, x);
	}
	printf("\n");
}

void adicionar_estado(char mapa[TAM_MAPA][TAM_MAPA], int linha, int coluna, ESTADO estado){
	if(coluna > 0 && !verifica_estado(mapa, linha, coluna - 1, CONHECIDO))
		mapa[linha][coluna - 1] |= estado;
	if(coluna < TAM_MAPA && !verifica_estado(mapa, linha, coluna + 1, CONHECIDO))
		mapa[linha][coluna + 1] |= estado;
	if(linha > 0 && !verifica_estado(mapa, linha - 1, coluna, CONHECIDO))
		mapa[linha - 1][coluna] |= estado;
	if(linha < TAM_MAPA && !verifica_estado(mapa, linha + 1, coluna, CONHECIDO))
		mapa[linha + 1][coluna] |= estado;
}

void retirar_estado(char mapa[TAM_MAPA][TAM_MAPA], int linha, int coluna, ESTADO estado){
	if(coluna > 0)
		mapa[linha][coluna - 1] &= ~estado;
	if(coluna < TAM_MAPA)
		mapa[linha][coluna + 1] &= ~estado;
	if(linha > 0)
		mapa[linha - 1][coluna] &= ~estado;
	if(linha < TAM_MAPA)
		mapa[linha + 1][coluna] &= ~estado;
}