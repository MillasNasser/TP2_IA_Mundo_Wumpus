#include "agente.h"

char salva_retorno[16] = {0};

void inicializa_jogador(){
	iniciar_mapa(player.mundo_conhecido, 0);
	player.mundo_conhecido[TAM_MAPA - 1][0] = mapa[TAM_MAPA - 1][0];
	player.direcao = NORTE;
	player.mundo_conhecido[TAM_MAPA - 1][0] = JOGADOR;
	player.linha = TAM_MAPA - 1;
	player.coluna = 0;
	mapa[TAM_MAPA-1][0] = JOGADOR;
	player.pontos = 0; // definindo pontuação inicial.
}

void pontuar(int pontos){

	player.pontos += pontos;
}

void andar(SENTIDO sentido){

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
	}
	pontuar(THROW_ARROW);
}

void agir(ACAO acao, SENTIDO sentido){

	switch(acao){

		case ANDAR:
			andar(sentido);
			break;
		case ATIRAR:
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

/*Sempre inicia no 0 o nivel da função, logo passe apenas 0*/
ACAO gera_acao(int nivel){
	int i, j;
	int livre = 0, wumpus = 0, poco = 0;
	int pai_coluna = 1, pai_linha = 1;

	int linha = player.linha;
	int coluna = player.coluna;

	//Pega todos os atributos da posição atual
	player.mundo_conhecido[player.linha][player.coluna] =
		mapa[player.linha][player.coluna];

	/* Se a posição do agente não foi visitada, a define
	 * como visitado */
	if(!verifica_estado(player.mundo_conhecido, linha, coluna, VISITADO)){
		player.mundo_conhecido[linha][coluna] &= ~CONHECIDO; //Isso aqui não pode existir.
		player.mundo_conhecido[linha][coluna] |= VISITADO;
	}

	marcar_estados_adj();

	//Adicionando a matriz que faz a manutenção no estado
	char matriz_estado[3][TAM_MAPA] = {
		{0}};

	//Se o nivel for diferente de 0, indica que ele tem pai
	if(nivel != 0){
		//Captura o sentido do jogador
		switch(player.direcao){
			case NORTE:
				pai_linha = -1;
				pai_coluna = 0;
				break;
			case LESTE:
				pai_linha = 0;
				pai_coluna = 1;
				break;
			case SUL:
				pai_linha = 1;
				pai_coluna = 0;
				break;
			case OESTE:
				pai_linha = 0;
				pai_coluna = -1;
				break;
		}
		matriz_estado[0][livre++] = (linha + pai_linha) * 10 + (coluna + pai_coluna); //Captura o pai 
	}

	/*Monta a matriz com as possibilidades de movimento*/
	for(i = -1; i <= 1; i++){
		for(j = -1; j <= 1; j++){
			if(i == j || i + j == 0 || (i == pai_linha && j == pai_coluna)) continue;
			printf("%d %d: %d\n", (linha + i), (coluna + j), (linha + i)*10 + (coluna + j));
			if(verifica_estado(player.mundo_conhecido, linha + i, coluna + j, WUMPUS)){
				matriz_estado[1][wumpus++] = (linha + i)*10 + (coluna + j);
			}else if(verifica_estado(player.mundo_conhecido, linha + i, coluna + j, POCO)){
				matriz_estado[2][poco++] = (linha + i)*10 + (coluna + j);
			}else if(verifica_estado(player.mundo_conhecido, linha + i, coluna + j, TODOS_ESTADOS)){
				matriz_estado[0][livre++] = (linha + i)*10 + (coluna + j);
			}
		}
	}
	printf("\n");

	for(i = 0; i < 3; i++){
		printf("%d: ", i);
		for(j = 0; j < 4; j++){
			printf("%d ", matriz_estado[i][j]);
		}
		printf("\n");
	}

	return 0;
}