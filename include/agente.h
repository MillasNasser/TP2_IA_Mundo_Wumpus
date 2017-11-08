#ifndef _AGENTE_H_
#define _AGENTE_H_

#define TAM_MAPA 4

typedef enum{
	V_ESQ, V_DIR, ANDAR, ATIRAR, PEGAR
}ACAO;

typedef enum{
	NORTE, LESTE, SUL, OESTE
}SENTIDO;

typedef struct s_Personagem{
	int x,y;
	int contador_movimento;
	int direcao;
	char mundo_conhecido[TAM_MAPA][TAM_MAPA];
}Personagem;

Personagem player;

/*TO-DO: Faz a movimentação do personagem
 *		 gastando pontos dependendo do movimento*/
void agir(ACAO acao);

/*Define a acção que o agente irá tomar*/
ACAO gera_acao();

void inicializa_jogador(int posicao_x, int posicao_y);

void imprime_mundo_conhecido();

#endif /*_AGENTE_H_*/