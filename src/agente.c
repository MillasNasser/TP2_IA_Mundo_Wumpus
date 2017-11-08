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

void mover(SENTIDO sentido){
        
    /* if que torna o ato de mover e 
     * e rotacionar atômico, rotacionando 
     * o player caso este não esteja na posição 
     * do movimento final. Evitando que seja 
     * necessário dividir o ato de mover em dois
     * pela função gera_acao. (descarta também o 
     * uso de memória.
    */
    if(sentido != player.direcao){
	
	rotacionar(sentido);
    }
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

// Função para alterar a pontuação do agente ao pegar o ouro.
void pegarOuro(){
    
    /*   
     * Falta definir como será a alteração do mapa
     * (remoção de ouro e também do wumpus).
     * A inclusão do cabeçalho ambiente.h neste modulo pode
     * ser realizada para que o mapa seja visível ao
     * jogador (aparentemente o módulo ambiente não esta uti-
     * -lizando recursos do módulo agente.h), 
     * porém o header agente.h já esta incluso
     * no header ambiente.h
     */
    pontuar(PREMIUM);
}

void atirarFlecha(){

    /*   
     * Falta definir como será a alteração do mapa
     * (remoção de ouro e também do wumpus).
     * A inclusão do cabeçalho ambiente.h neste modulo pode
     * ser realizada para que o mapa seja visível ao
     * jogador (aparentemente o módulo ambiente não esta uti-
     * -lizando recursos do módulo agente.h), 
     * porém o header agente.h já esta incluso
     * no header ambiente.h
     */
    pontuar(THROW_ARROW);
}

void agir(ACAO acao, SENTIDO sentido){
	
    switch(acao){
	
	/*case ROTATE:
	    rotacionar(sentido);
	    break;		*/
	case ANDAR:
	    mover(sentido);
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

void retirar_estado(ESTADO estado){
	int x = player.x;
	int y = player.y;
	if(x > 0)
		player.mundo_conhecido[y][x-1] &= ~estado;
	if(x < TAM_MAPA)
		player.mundo_conhecido[y][x+1] &= ~estado;
	if(y > 0)
		player.mundo_conhecido[y-1][x] &= ~estado;
	if(y < TAM_MAPA)
		player.mundo_conhecido[y+1][x] &= ~estado;
}

void adicionar_estado(ESTADO estado){
	int x = player.x;
	int y = player.y;
	if(x > 0 && !verifica_estado(player.mundo_conhecido,y,x-1,CONHECIDO))
		player.mundo_conhecido[y][x-1] |= estado;
	if(x < TAM_MAPA && !verifica_estado(player.mundo_conhecido,y,x+1,CONHECIDO))
		player.mundo_conhecido[y][x+1] |= estado;
	if(y > 0 && !verifica_estado(player.mundo_conhecido,y-1,x,CONHECIDO))
		player.mundo_conhecido[y-1][x] |= estado;
	if(y < TAM_MAPA && !verifica_estado(player.mundo_conhecido,y+1,x,CONHECIDO))
		player.mundo_conhecido[y+1][x] |= estado;
}

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
	player.mundo_conhecido[player.y][player.x] = 
			mapa[player.y][player.x];
	if(verifica_estado(player.x,player.y,FEDOR)){
		
	}
	return 0;
}