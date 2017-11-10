#include "agente.h"

char salva_retorno[16] = {0};

void __preenche_mundo_conhecido(int valor){
	int i,j;
	for(i=0; i < TAM_MAPA; i++){
		for(j = 0; j < TAM_MAPA; j++){
			player.mundo_conhecido[i][j] = valor;
		}
	}
	player.mundo_conhecido[3][0] = mapa[3][0];
}

void inicializa_jogador(){
	__preenche_mundo_conhecido(0);
	player.direcao = NORTE;
	player.mundo_conhecido[3][0] = JOGADOR;
	player.y = 3; player.x = 0;
	mapa[3][0] = JOGADOR;
    player.pontos = 0; // definindo pontuação inicial
}					   // para teste.

void pontuar(int pontos){
        
    player.pontos += pontos;    
}

void andar(SENTIDO sentido){
        
	rotacionar(sentido);	
    switch(player.direcao){
	
		case NORTE:
			player.y--;
			break;
		case LESTE:
			player.x++;
			break;
		case SUL:
			player.y++;
			break;
		case OESTE:
			player.x--;
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
	int x = player.x, y = player.y;
    rotacionar(sentido);	
	
	switch(sentido){
		
		case NORTE:			
			y--;
			break;
		case SUL:
			y++;
			break;
		case LESTE:
			x++;
			break;
		case OESTE:
			x--;
			break;
		default:
			break;
	}
	if(verifica_estado(mapa, y, x, WUMPUS)){	

		retirar_estado(mapa, x, y, WUMPUS);
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

		default :
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
	int x = player.x;
	int y = player.y;
	/*Estados adjacentes*/
	if(x > 0 ) //OESTE
		player.mundo_conhecido[y][x-1] &= ~estado;
	if(x < TAM_MAPA) //LESTE
		player.mundo_conhecido[y][x+1] &= ~estado;
	if(y > 0) //NORTE
		player.mundo_conhecido[y-1][x] &= ~estado;
	if(y < TAM_MAPA) //SUL
		player.mundo_conhecido[y+1][x] &= ~estado;
}

/*Adiciona suposições a estados que estão adjacentes à posição que o agente está*/
/*TO-DO: Não alterar estados que são visitados*/
void ag_adicionar_estado(ESTADO estado){
	int x = player.x;
	int y = player.y;
	/*Estados adjacentes*/
	if(x > 0 && !verifica_estado(player.mundo_conhecido,y,x-1,CONHECIDO|VISITADO))//OESTE
		player.mundo_conhecido[y][x-1] |= estado;
	if(x < TAM_MAPA && !verifica_estado(player.mundo_conhecido,y,x+1,CONHECIDO|VISITADO))//LESTE
		player.mundo_conhecido[y][x+1] |= estado;
	if(y > 0 && !verifica_estado(player.mundo_conhecido,y-1,x,CONHECIDO|VISITADO))//NORTE
		player.mundo_conhecido[y-1][x] |= estado;
	if(y < TAM_MAPA && !verifica_estado(player.mundo_conhecido,y+1,x,CONHECIDO|VISITADO))//SUL
		player.mundo_conhecido[y+1][x] |= estado;
}

/*Define as respectivas suposições em relação a posição a que o agente está presente*/
int marcar_estados_adj(){
	int estado = 0;
	int x = player.x, 
		y = player.y;

	//Verifica se na posição possui brisa ou fedor
	for(estado = 1; estado < 4; estado*=2){
		//Se não houver retire dos que estão em volta
		if(!verifica_estado(player.mundo_conhecido,y,x,estado)){
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
	int i,j;
	int livre = 0, wumpus = 0, poco = 0;
	int pai_x = 1, pai_y = 1;

	int x = player.x,
		y = player.y;

	//Pega todos os atributos da posição atual
	player.mundo_conhecido[player.y][player.x] = 
			mapa[player.y][player.x];

	/* Se a posição do agente não foi visitado, a define
	 * como visitado */
	if(!verifica_estado(player.mundo_conhecido,y,x,VISITADO)){
		player.mundo_conhecido[y][x] &= ~CONHECIDO;
		player.mundo_conhecido[y][x] |= VISITADO;
	}

	marcar_estados_adj();

	///Adicionando a matriz que faz a manutenção no estado
	char matriz_estado[3][TAM_MAPA]= {{0}};

	///Se o nivel for diferente de 0, indica que ele tem pai
	if(nivel != 0){
		//Captura o sentido do jogador
		switch(player.direcao){
			case NORTE: pai_y =-1; pai_x = 0;	break;
			case LESTE: pai_y = 0; pai_x = 1;	break;
			case SUL:   pai_y = 1; pai_x = 0;	break;
			case OESTE: pai_y = 0; pai_x =-1;	break;
		}
		matriz_estado[0][livre++] = (y+pai_y)*10+(x+pai_x);///Captura o pai 
	}

	/*Monta a matriz com as possibilidades de movimento*/
	for(i = -1; i <= 1; i++){
		for(j = -1; j <= 1; j++){
			if(i==j || i+j == 0 || (i == pai_y && j == pai_x)) continue;
			printf("%d %d: %d\n",(y+i), (x+j),(y+i)*10 + (x+j));
			if(verifica_estado(player.mundo_conhecido,y+i,x+j, WUMPUS)){
				matriz_estado[1][wumpus++] = (y+i)*10 + (x+j);
			}else if(verifica_estado(player.mundo_conhecido,y+i,x+j, POCO)){
				matriz_estado[2][poco++] = (y+i)*10 + (x+j);
			}else if(verifica_estado(player.mundo_conhecido,y+i,x+j,TODOS_ESTADOS)){
				matriz_estado[0][livre++] = (y+i)*10 + (x+j);
			}
		}
	}printf("\n");

	for(i = 0; i < 3; i++){
		printf("%d: ",i);
		for(j = 0; j < 4; j++){
			printf("%d ",matriz_estado[i][j]);
		}printf("\n");
	}

	return 0;
}