#include "agente.h"

void inicializa_jogador(){
    
    player.pontos = 0; // definindo pontuação inicial
}			// para teste.

ACAO gera_acao(){
	return 0;
}

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
	
	case ROTATE:
	    rotacionar(sentido);
	    break;		
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
	
}
