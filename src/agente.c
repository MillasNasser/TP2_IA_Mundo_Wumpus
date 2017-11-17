#include "agente.h"

char ACAO_STR[3][10] = {"ANDAR", "ATIRAR", "PEGAR"};

char SENTIDO_STR[5][10] = {"NORTE", "LESTE", "SUL", "OESTE", " "};

void inicializa_jogador(){
	iniciar_mapa(player.mundo_conhecido, 0);
	mapa[TAM_MAPA - 1][0] |= (JOGADOR | VISITADO);
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

	adicionar_estado(mapa, player.linha, player.coluna, JOGADOR, TODOS_ESTADOS);
	adicionar_estado(mapa, player.linha, player.coluna, VISITADO, TODOS_ESTADOS);

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
	finaliza("Achou o Ouro! Fim de jogo");
}

void atirarFlecha(SENTIDO sentido){
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

		remover_estados_adjacentes(player.mundo_conhecido, linha, coluna, FEDOR);
	}
	remover_estado(player.mundo_conhecido, linha, coluna, WUMPUS);
	player.flecha--;
	pontuar(THROW_ARROW);
}

void agir(ACAO acao, SENTIDO sentido){

	printf("Ação: %s %s", ACAO_STR[acao], (sentido < 4) ? SENTIDO_STR[sentido] : SENTIDO_STR[5]);
	//getchar();
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
			finaliza("Não fui programado para agir desse jeito");
			break;
	}
}

void imprime_mundo_conhecido(){
	imprime_mapa(player.mundo_conhecido);
	printf("Posicao: (%d, %d)\n", player.linha, player.coluna);
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
			remover_estados_adjacentes(player.mundo_conhecido, linha, coluna, estado * 4);
		}else{
			adicionar_estados_adjacentes(player.mundo_conhecido, linha, coluna, estado * 4, CONHECIDO);

			/*
			 * Como há somente 1 wumpus no mapa, caso o estado seja FEDOR, é
			 * preciso remover todos os supostos wumpus que não estejam
			 * adjacentes a algum fedor descoberto.
			 */
			if(estado == FEDOR){
				int i, j;
				for(i = 0; i < TAM_MAPA; i++){
					for(j = 0; j < TAM_MAPA; j++){
						if(verifica_estado(player.mundo_conhecido, i, j, WUMPUS)){
							if(MODULO(i - linha) + MODULO(j - coluna) > 1){
								printf("1- Wumpus em (%d, %d) não é adjacente ao fedor em (%d,%d)\n", i, j, linha, coluna);
								remover_estado(player.mundo_conhecido, i, j, WUMPUS);
							}
						}
						if(verifica_estado(player.mundo_conhecido, i, j, FEDOR)){
							int k, l;
							for(k = -1; k <= 1; k++){
								for(l = -1; l <= 1; l++){
									if(MODULO(k) + MODULO(l) == 1){
										if(verifica_estado(player.mundo_conhecido, linha + k, coluna + l, WUMPUS)){
											if(MODULO(i - (linha + k)) + MODULO(j - (coluna + l)) > 1){
												printf("2- Wumpus em (%d, %d) não é adjacente ao fedor em (%d,%d)\n", linha + k, coluna + l, i, j);
												remover_estado(player.mundo_conhecido, linha + k, coluna + l, WUMPUS);
											}
										}
									}
								}
							}
						}
					}
				}
			}

		}
	}
	adicionar_estados_adjacentes(player.mundo_conhecido, linha, coluna, CONHECIDO, TODOS_ESTADOS);
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
			//printf("converte_hash_para_sentido: %d não é adjacente à %d\n", hash_jogador, hash);
			finaliza("caiu no poço!");
			return -1;
	}
}

void pm(char matriz_estado[3][TAM_MAPA]){
	int i, j;
	for(i = 0; i < 3; i++){
		for(j = 0; j < TAM_MAPA; j++){
			printf("%02d ", matriz_estado[i][j]);
		}
		printf("\n");
	}
}

int gera_acao(char pai[TAM_MAPA * TAM_MAPA], int ultimo){
	int i, j, k;
	int livre = 0, wumpus = 0, poco = 0;
	char hash_novo;
	char hash_player = hash(player.linha, player.coluna);
	int retorno;

	//Encontrando o último pai.
	char ultimo_pai;
	printf("Caminho:");
	for(k = 0; k < TAM_MAPA * TAM_MAPA && pai[k] != -1; k++){
		printf(" %02d", pai[k]);
	}
	ultimo_pai = k - 1;

	//Criando a matriz de movimentos.
	char matriz_estado[3][TAM_MAPA];
	for(i = 0; i < 3; i++){
		for(j = 0; j < TAM_MAPA; j++){
			matriz_estado[i][j] = -1;
		}
	}

	printf("\n---=== Início ===---\n");
	printf("MAPA GLOBAL:\n");
	imprime_mapa(mapa);
	printf("MUNDO CONHECIDO\n");
	imprime_mundo_conhecido();
	printf("Último: %s\n", ultimo ? "sim" : "não");
	printf("Pai: %d\n", pai[ultimo_pai]);
	pm(matriz_estado);
	//getchar();

	if(verifica_estado(mapa, player.linha, player.coluna, RELUSENTE)){
		agir(PEGAR, -1);
	}else if(verifica_estado(mapa, player.linha, player.coluna, POCO)){
		pontuar(-1000);
		finaliza("Caiu no poço");
	}else if(verifica_estado(mapa, player.linha, player.coluna, WUMPUS)){
		pontuar(-1000);
		finaliza("Wumpus te matou!");
	}

	marcar_estados_adj();

	printf("#####%d %d %d#####\n", hash_player, player.linha, player.coluna);

	/*Monta a matriz com as possibilidades de movimento*/
	for(i = -1; i <= 1; i++){
		for(j = -1; j <= 1; j++){

			if(i == j || i + j == 0){
				goto fim;
			}

			hash_novo = hash(player.linha + i, player.coluna + j);

			//Verifica se o movimento não está na lista de pais.
			for(k = 0; k < TAM_MAPA * TAM_MAPA && pai[k] != -1; k++){
				if(hash_novo == pai[k]){
					goto fim;
				}
			}

			if(verifica_estado(player.mundo_conhecido, player.linha + i, player.coluna + j, WUMPUS)){
				matriz_estado[1][wumpus++] = hash_novo;
				printf("add[%d][%d]: %d WUMPUS\n", 1, wumpus - 1, hash_novo);
			}else if(verifica_estado(player.mundo_conhecido, player.linha + i, player.coluna + j, POCO)){
				matriz_estado[2][poco++] = hash_novo;
				printf("add[%d][%d]: %d POÇO\n", 2, poco - 1, hash_novo);
			}else if(verifica_estado(player.mundo_conhecido, player.linha + i, player.coluna + j, NENHUM_ESTADO)){
				matriz_estado[0][livre++] = hash_novo;
				printf("add[%d][%d]: %d LIVRE\n", 0, livre - 1, hash_novo);
			}
			fim:;
		}
	}

	int esconde_bug;
	SENTIDO sentido;
	//Percorre Matriz.
	for(i = 0; i < 3; i++){
		for(j = 0; j < TAM_MAPA && matriz_estado[i][j] != -1; j++){
			hash_novo = matriz_estado[i][j];

			if(hash_novo == -1){
				continue;
			}

			esconde_bug = 0;
			printf("---=== Antes da ação ===---\n");
			printf("MAPA GLOBAL:\n");
			imprime_mapa(mapa);
			printf("MUNDO CONHECIDO\n");
			imprime_mundo_conhecido();
			printf("Último: %s\n", ultimo ? "sim" : "não");
			printf("Pai: %d\n", pai[ultimo_pai]);
			pm(matriz_estado);

			for(int k = 0; pai[k] != -1; k++){
				printf("%d ", pai[k]);
			}
			printf("\n");

			sentido = converte_hash_para_sentido(hash_novo);

			//Ainda tem estados livres.
			if(i == 0 && j < livre){
				//printf("Ação: ANDAR ");
				agir(ANDAR, sentido);
				//Adiciona a si próprio na lista de pais.
				pai[ultimo_pai + 1] = hash_player;
			}else{
				if(i == 0){
					i++;
				}
				//Verifica se tem wumpus.
				if(i == 1 && j < wumpus){
					if(player.flecha > 0 && ultimo){
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
							//printf("Ação: ATIRAR %s\n", )
							agir(ATIRAR, sentido);

							//Atualizando matriz.
							matriz_estado[0][livre++] = hash_novo;
							matriz_estado[1][j] = -1;
							wumpus--;
							i = 0;
							j = livre - 2;
							esconde_bug = 1;
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
							sentido = converte_hash_para_sentido(matriz_estado[2][rand() % poco]);
							agir(ANDAR, sentido);

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

			printf("---=== Depois da ação ===---\n");
			printf("MAPA GLOBAL:\n");
			imprime_mapa(mapa);
			printf("MUNDO CONHECIDO\n");
			imprime_mundo_conhecido();
			printf("Último: %s\n", ultimo ? "sim" : "não");
			printf("Pai: %d\n", pai[ultimo_pai]);
			pm(matriz_estado);

			//Chama a recursão.
			//TO-DO: consertar essa parte.
			if(ultimo == 0 || (i == 0 && (j + 1 + esconde_bug < livre || (wumpus > 0 && player.flecha > 0)))){
				retorno = gera_acao(pai, 0);
			}else{
				retorno = gera_acao(pai, 1);
			}

			if(retorno == 1){
				//Atualizando matriz.
				matriz_estado[1][wumpus++] = hash_novo;
			}else if(retorno == 0){
				//Atualizando matriz.
				matriz_estado[2][poco++] = hash_novo;
			}
		}
	}

	//Volta o movimento.
	sentido = converte_hash_para_sentido(pai[ultimo_pai]);
	agir(ANDAR, sentido);
	pai[ultimo_pai] = -1;
	return -1;
}

void finaliza(const char *mensagem){
	if(mensagem != NULL){
		puts(mensagem);
	}
	
	printf("Pontuação final: %d\n", player.pontos);
	
}
