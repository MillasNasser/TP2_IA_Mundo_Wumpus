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

void __imprime_mundo_conhecido_linha(char hor, char vert, int x){
	int i;
	for(i = 0; i < (TAM_MAPA * x) + TAM_MAPA + 1; i++){
		printf("%c", (i % TAM_MAPA) ? hor : vert);
	}
	printf("\n");
}

void imprime_mundo_conhecido(){
	int i, j, k, l;
	int x = 3, y = 2;
	char vert = ':', hor = '-';
	int bit;

	char simbolos[7] = {' ', 'B', 'P', 'F', 'W', 'R', 'J'};
	
	__imprime_mundo_conhecido_linha(hor, vert, x);
	for(i=0; i<TAM_MAPA; i++){
		for(l=0; l<y; l++){
			printf("%c", vert);
			for(j=0; j<TAM_MAPA; j++){
				for(k=0; k<x; k++){
					bit = k + (l*x);
					printf("%c", simbolos[ (bit+1) * verifica_estado(i, j, 1 << bit) ]);
				}
				printf("%c", vert);
			}
			printf("\n");
		}
		__imprime_mundo_conhecido_linha(hor, vert, x);
	}
	printf("\n");
}

int marcar_estados_adj(){
	int i, j, estado = 0;
	int x = player.x;
	int y = player.y;
	if(verifica_estado(y,x,WUMPUS|POCO)){
		return 0;
	}
	for(estado = 1; estado < 32; estado <<= 1){
		if(estado == POCO|WUMPUS|RELUSENTE ||
		   !verifica_estado(y,x,estado)) continue;
		if(x > 0)
			player.mundo_conhecido[y][x-1] |= estado<<1;
		if(x < 3)
			player.mundo_conhecido[y][x+1] |= estado*2;
		if(y > 0)
			player.mundo_conhecido[y-1][x] |= estado;
		if(y < 3)
			player.mundo_conhecido[y+1][x] |= estado;
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