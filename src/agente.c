#include "agente.h"

char salva_retorno[16] = {0};

void inicializa_jogador(){
	iniciar_mapa(player.mundo_conhecido, 0);
	mapa[TAM_MAPA - 1][0] |= (JOGADOR | VISITADO) ;
	player.mundo_conhecido[TAM_MAPA - 1][0] = mapa[TAM_MAPA - 1][0] | CONHECIDO;
	player.direcao = NORTE;
	player.linha = TAM_MAPA - 1;
	player.coluna = 0;
	player.pontos = 0; // definindo pontuação inicial.
	player.flecha = 1;
}

void pontuar(int pontos){

	player.pontos += pontos;
}

void andar(SENTIDO sentido){

	remover_estado(mapa, player.linha, player.coluna, JOGADOR);
	remover_estado(player.mundo_conhecido, player.linha, player.coluna, JOGADOR);
	
	rotacionar(sentido);
	switch(player.direcao){

		case NORTE:
			player.linha--;
			break;
		case LESTE:
			player.coluna++;
			break;
		case SUL:
			player.linha++;
			break;
		case OESTE:
			player.coluna--;
			break;
	}
	pontuar(-1);
	
	adicionar_estado(mapa, player.linha, player.coluna, JOGADOR);
	adicionar_estado(mapa, player.linha, player.coluna, VISITADO);

	//Pega todos os atributos da posição atual
	player.mundo_conhecido[player.linha][player.coluna] =
		mapa[player.linha][player.coluna];
	
	player.mundo_conhecido[player.linha][player.coluna] |= CONHECIDO;

}

void rotacionar(SENTIDO newSentido){

	if(newSentido != player.direcao){

		if(MODULO(player.direcao - newSentido) == 2){
			// Se for preciso movimento de 180° 
			// penalizr o agente em 2 pontos.
			pontuar(-2);
		}else{
			// se nao penalizar o agente em 1.
			pontuar(-1);
		}
		player.direcao = newSentido;
	}
}

// Função para alterar a pontuação do agente ao pegar o ouro.

void pegarOuro(){

	pontuar(PREMIUM);

	//TO-DO: melhorar isso.
	printf("Fim de jogo\n");
	exit(0);
}

void atirarFlecha(SENTIDO sentido){

	if(!player.flecha){

		return;
	}
	int linha = player.linha;
	int coluna = player.coluna;
	rotacionar(sentido);

	switch(sentido){

		case NORTE:
			linha--;
			break;
		case SUL:
			linha++;
			break;
		case LESTE:
			coluna++;
			break;
		case OESTE:
			coluna--;
			break;
		default:
			break;
	}
	if(verifica_estado(mapa, linha, coluna, WUMPUS)){

		remover_estado(mapa, linha, coluna, WUMPUS);
		remover_estados_adjacentes(mapa, linha, coluna, FEDOR);
		
		remover_estado(player.mundo_conhecido, linha, coluna, WUMPUS);
		remover_estados_adjacentes(player.mundo_conhecido, linha, coluna, FEDOR);
	}
	pontuar(THROW_ARROW);
}

void agir(ACAO acao, SENTIDO sentido){

	switch(acao){

		case ANDAR:
			andar(sentido);
			break;
		case ATIRAR:
			atirarFlecha(sentido);
			break;
		case PEGAR:
			pegarOuro();
			break;

		default:
			/*tratamento de erro*/
			break;
	}
}

void imprime_mundo_conhecido(){
	imprime_mapa(player.mundo_conhecido);
	printf("Posicao: (%d, %d)\n", player.linha, player.coluna);
}

/*Remove suposições a estados que estão adjacentes à posição que o agente está*/

/*TO-DO: Não alterar estados que são visitados*/
void ag_retirar_estado(ESTADO estado){
	int linha = player.linha;
	int coluna = player.coluna;
	/*Estados adjacentes*/
	if(coluna > 0) //OESTE
		player.mundo_conhecido[linha][coluna - 1] &= ~estado;
	if(coluna < TAM_MAPA) //LESTE
		player.mundo_conhecido[linha][coluna + 1] &= ~estado;
	if(linha > 0) //NORTE
		player.mundo_conhecido[linha - 1][coluna] &= ~estado;
	if(linha < TAM_MAPA) //SUL
		player.mundo_conhecido[linha + 1][coluna] &= ~estado;
}

/*Adiciona suposições a estados que estão adjacentes à posição que o agente está*/

/*TO-DO: Não alterar estados que são visitados*/
void ag_adicionar_estado(ESTADO estado){
	int linha = player.linha;
	int coluna = player.coluna;
	/*Estados adjacentes*/
	if(coluna > 0 && !verifica_estado(player.mundo_conhecido, linha, coluna - 1, CONHECIDO | VISITADO))//OESTE
		player.mundo_conhecido[linha][coluna - 1] |= estado;
	if(coluna < TAM_MAPA && !verifica_estado(player.mundo_conhecido, linha, coluna + 1, CONHECIDO | VISITADO))//LESTE
		player.mundo_conhecido[linha][coluna + 1] |= estado;
	if(linha > 0 && !verifica_estado(player.mundo_conhecido, linha - 1, coluna, CONHECIDO | VISITADO))//NORTE
		player.mundo_conhecido[linha - 1][coluna] |= estado;
	if(linha < TAM_MAPA && !verifica_estado(player.mundo_conhecido, linha + 1, coluna, CONHECIDO | VISITADO))//SUL
		player.mundo_conhecido[linha + 1][coluna] |= estado;
}

/*Define as respectivas suposições em relação a posição a que o agente está presente*/
int marcar_estados_adj(){
	int estado = 0;
	int linha = player.linha;
	int coluna = player.coluna;

	//Verifica se na posição possui brisa ou fedor
	for(estado = 1; estado < 4; estado *= 2){
		//Se não houver retire dos que estão em volta
		if(!verifica_estado(player.mundo_conhecido, linha, coluna, estado)){
			ag_retirar_estado(estado * 4);
			ag_adicionar_estado(CONHECIDO);
			//Se houver verifica se não foi retirado
		}else{
			ag_adicionar_estado(estado * 4);
		}
	}
	return 1;
}

SENTIDO converte_hash_para_sentido(char hash){
	char hash_jogador = hash(player.linha, player.coluna);
	
	switch(hash - hash_jogador){
		case -10:
			return NORTE;
		case 10:
			return SUL;
		case 1:
			return LESTE;
		case -1:
			return OESTE;
		default:
			printf("converte_hash_para_sentido: erro\n");
			exit(0);
	}
}

void pm(char matriz_estado[3][TAM_MAPA]){
	int i, j;
	for(i=0; i<3; i++){
		for(j=0; j<TAM_MAPA; j++){
			printf("%02d ", matriz_estado[i][j]);
		}
		printf("\n");
	}
}

/*Sempre inicia no 0 o nivel da função, logo passe apenas 0*/
int gera_acao(char pai[TAM_MAPA * TAM_MAPA], int ultimo){
	int i, j, k;
	int livre = 0, wumpus = 0, poco = 0;
	char hash_novo;
	char hash_player = hash(player.linha, player.coluna);
	int retorno;

	if(verifica_estado(mapa, player.linha, player.coluna, RELUSENTE)){
		agir(PEGAR, -1);
	}else if(verifica_estado(mapa, player.linha, player.coluna, POCO)){
		//TO-DO
		printf("Perdeu\n");
		exit(0);
	}

	marcar_estados_adj();

	//Adicionando a matriz que faz a manutenção no estado
	char matriz_estado[3][TAM_MAPA];
	
	for(i=0; i<3; i++){
		for(j=0; j<TAM_MAPA; j++){
			matriz_estado[i][j] = -1;
		}
	}

	//Encontrando o último pai.
	char ultimo_pai;
	for(k = 0; k < TAM_MAPA * TAM_MAPA && pai[k] != -1; k++);
	ultimo_pai = k - 1;

	imprime_mapa(mapa);
	imprime_mundo_conhecido();
	
	printf("Ultimo: %s\n", ultimo?"sim":"nao");
	printf("Pai: %d\n", pai[ultimo_pai]);
	
	/*Monta a matriz com as possibilidades de movimento*/
	for(i = -1; i <= 1; i++){
		for(j = -1; j <= 1; j++){
			
			if(i == j || i + j == 0){
				goto fim;
			}

			//hash_novo = (linha + i) * 10 + coluna + j;
			hash_novo = hash(player.linha + i, player.coluna + j);

			//Verifica se o movimento não está na lista de pais.
			for(k = 0; k < TAM_MAPA * TAM_MAPA && pai[k] != -1; k++){
				if(k < 0 || hash_novo == pai[k]){
					goto fim;
				}
			}

			//printf("%d %d: %d\n", (linha + i), (coluna + j), hash_novo);
			if(verifica_estado(player.mundo_conhecido, player.linha + i, player.coluna + j, WUMPUS)){
				matriz_estado[1][wumpus++] = hash_novo;
			}else if(verifica_estado(player.mundo_conhecido, player.linha + i, player.coluna + j, POCO)){
				matriz_estado[2][poco++] = hash_novo;
			}else if(verifica_estado(player.mundo_conhecido, player.linha + i, player.coluna + j, TODOS_ESTADOS)){
				matriz_estado[0][livre++] = hash_novo;
			}
			fim:;
		}
	}
	
	//getchar();
	//exit(0);

	SENTIDO sentido;
	//Percorre Matriz.
	for(i = 0; i < 3; i++){
		for(j = 0; j < TAM_MAPA; j++){
			hash_novo = matriz_estado[i][j];
			
			if(hash_novo == -1){
				continue;
			}
			
			printf("---------------------------\n");
			printf("%d (%d, %d)\nMAPA GLOBAL:\n", ultimo_pai, i, j);
			imprime_mapa(mapa);
			printf("MUNDO CONHECIDO\n");
			imprime_mundo_conhecido();
			printf("Ultimo: %s\n", ultimo?"sim":"nao");
			printf("Pai: %d\n", pai[ultimo_pai]);
			pm(matriz_estado);
			
			sentido = converte_hash_para_sentido(hash_novo);

			//Ainda tem estados livres.
			if(i == 0 && j < livre){
				agir(ANDAR, sentido);
				//Adiciona a si próprio na lista de pais.
				pai[ultimo_pai + 1] = hash_player;
			}else{
				if(i == 0){
					i++;
				}
				//Verifica se tem wumpus.
				if(i == 1 && j < wumpus){
					if(ultimo){
						int offset_linha = player.linha;
						int offset_coluna = player.coluna;
						switch(sentido){
							case NORTE:
								offset_linha--;
								break;
							case SUL:
								offset_linha++;
								break;
							case LESTE:
								offset_coluna++;
								break;
							case OESTE:
								offset_coluna--;
								break;
						}
						if(verifica_estado(player.mundo_conhecido, offset_linha, offset_coluna, WUMPUS)){
							agir(ATIRAR, sentido);

							//Atualizando matriz.
							matriz_estado[0][livre++] = hash_novo;
							matriz_estado[1][--wumpus] = -1;
							i = 0;
							j = livre - 2;
						}else{
							agir(ANDAR, sentido);
							//Adiciona a si próprio na lista de pais.
							pai[ultimo_pai + 1] = hash_player;
						}
					}else{
						//Volta o movimento.

						sentido = converte_hash_para_sentido(pai[ultimo_pai]);
						agir(ANDAR, sentido);
						pai[ultimo_pai] = -1;
						return 1;
					}
				}else{
					if(i == 1){
						i++;
					}
					if(i == 2){
						//Só tem poço.
						if(ultimo){
							sentido = converte_hash_para_sentido(rand() % poco);
							agir(ANDAR, sentido);

							//TO-DO: talvez atualizar a matriz.
							
							//Adiciona a si próprio na lista de pais.
							pai[ultimo_pai + 1] = hash_player;

						}else{
							//Volta o movimento.

							sentido = converte_hash_para_sentido(pai[ultimo_pai]);
							agir(ANDAR, sentido);
							pai[ultimo_pai] = -1;
							return 0;
						}
					}
				}
			}

			//Chama a recursão.
			//TO-DO: consertar essa parte.
			if(ultimo == 0 || (i == 0 && (j + 1 < livre || wumpus > 0))){
				retorno = gera_acao(pai, 0);
			}else{
				retorno = gera_acao(pai, 1);
			}
			
			if(retorno == 1){
				//Atualizando matriz.
				matriz_estado[1][wumpus++] = hash_novo;
			}else{
				//Atualizando matriz.
				matriz_estado[2][poco++] = hash_novo;
			}
		}
	}

	return 0;
}