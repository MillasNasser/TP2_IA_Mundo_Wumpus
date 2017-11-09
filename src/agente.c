#include "agente.h"

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
	player.mundo_conhecido[3][0] = JOGADOR;
	player.y = 3; player.x = 0;
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
		
		//TO-DO chamar a função de alterar estado para as 
		// casas vizinhas a posição (x,y) e para (x,y).
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
void retirar_estado(ESTADO estado){
	int x = player.x;
	int y = player.y;
	/*Estados adjacentes*/
	if(x > 0 && !verifica_estado(player.mundo_conhecido,y,x-1,VISITADO)) //OESTE
		player.mundo_conhecido[y][x-1] &= ~estado;
	if(x < TAM_MAPA && !verifica_estado(player.mundo_conhecido,y,x+1,VISITADO)) //LESTE
		player.mundo_conhecido[y][x+1] &= ~estado;
	if(y > 0 && !verifica_estado(player.mundo_conhecido,y-1,x,VISITADO))//NORTE
		player.mundo_conhecido[y-1][x] &= ~estado;
	if(y < TAM_MAPA && !verifica_estado(player.mundo_conhecido,y+1,x,VISITADO))//SUL
		player.mundo_conhecido[y+1][x] &= ~estado;
}

/*Adiciona suposições a estados que estão adjacentes à posição que o agente está*/
/*TO-DO: Não alterar estados que são visitados*/
void adicionar_estado(ESTADO estado){
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
			retirar_estado(estado * 4);
			adicionar_estado(CONHECIDO);
		//Se houver verifica se não foi retirado
		}else{
			adicionar_estado(estado * 4);
		}
	}
	return 1;
}

ACAO gera_acao(){
	int x = player.x,
		y = player.y;

	player.mundo_conhecido[player.y][player.x] = 
			mapa[player.y][player.x];
	//Se a posição do agente não foi visitado
	if(!verifica_estado(player.mundo_conhecido,y,x,VISITADO)){
		player.mundo_conhecido[y][x] &= ~CONHECIDO;
		player.mundo_conhecido[y][x] |= VISITADO;
	}
	if(verifica_estado(player.x,player.y,FEDOR)){
		
	}
	return 0;
}