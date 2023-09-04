#include <MKL25Z4.h>
#include "rotinas_viagem.h"
#include "controle_motor.h"
#include "menus.h"
#include "lcdio.h"
#include "teclado.h"
#include "timers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char estado_viagem[16] = "estacao_1"; //variavel q diz em que posição o trem está atualmente, começa na estacao 1
int tempoPortasAbertas = 0; //variavel q contabiliza o tempo q se passou com as portas abertas no modo manual

double parametros[3][4] =
	{{1300, 30, 20, 60},
	{2100, 30, 20, 70},
	{1700, 30, 30, 63}}; //parametros de distancia, tempo entre estacoes, tempo de parada e velocidade maxima em km/h.
	
int portas_abertas = 0; //variavel q diz se as portas estão abertas ou não
int tempoDeEspera = 0; //variavel q indica quantas vezes foi pressionado o botao de pedir mais tempo no modo automatico, para dps decrementar do tempo total da viagem
int ida = 1; //variavel q diz se o trem esta indo ou voltando, começa indo

void abrePortasEspera(){ //essa função abre as portas e ativa o timer 0 do PIT, fazendo o controle do tempo que as portas ficam abertas no modo AUTOMATICO. o canal 0 do PIT conta 12s e quando estoura ativa o canal 1, que conta 3s blinkando os leds e depois seta portas_abertas = 0;
	PORTA_PCR5 |= ((1<< 8) + 3) | (10 << 16); //ativa interrupção na coluna dos botoes da porta, teclas A e B
	portas_abertas=1;
	if(portas_abertas == 1){
		setup_PIT0(); //seta o canal 0 do pit, o canal 0 do PIT conta 12s e quando estoura ativa o canal 1, que conta 3s blinkando os leds e depois seta portas_abertas = 0;
		GPIOC_PSOR |= (1<<10 | 1<<11); //seta os leds das portas abertas
	}
	while(portas_abertas==1); //espera até o canal 1 do pit estourar e fechar as portas p seguir o codigo
	GPIOC_PCOR |= (1<<10 | 1<<11); //desliga os leds das portas
	PORTA_PCR5 &= ~(10 << 16); //desliga interrupção dos botoes de pedir mais tempo (A e B)
}

void tostring(char str[], int num) //função auxiliar q transforma int em string
{
    int i, rem, len = 0, n;
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}

void realizar_viagem(int estacao){ //
			estacao--; //diminui 1 para poder usar o valor da estação como indice na matriz "parametros"
		
	if(strcmp(estado, "modoAuto") == 0) //se esta no modo automatico, executa a rotina de abertura de portas e aguardar fechar p continuar, se está no manual as portas já foram fechadas anteriormente
	{
		abrePortasEspera();
	
	}
			atraso(parametros[estacao][2]-15, 's'); //espera o tempo necessario para começar a viagem
			float vel = (parametros[estacao][0])/((parametros[estacao][1])-5-tempoDeEspera*5); //calculo da velocidade , vel = (distancia/(tempo-5-5*apertosNoBotaoDePedirMaisTempo))
			int duracao_viagem = parametros[estacao][1] - tempoDeEspera*5; //tempo de espera é referente ao numero de apertos no botao p pedir mais tempo de portas abertas no modo automatico, esse tempo é agr decrementado aqui
			char str_duracao_viagem [10]; //string q é usada para atualizar a duracao da viagem
			
	//rampa subida:
			limpa_reseta_cursor();
			send_string("DURACAO VIAGEM:");
			for(int i=1;i <6; i++){ //faz a rampa de subida
				pwm_motor(vel*i);
				atraso(1, 's');
				duracao_viagem--;
				tostring(str_duracao_viagem, duracao_viagem); //transforma a duracao de viagem em string
				proxima_linha();
				send_string(str_duracao_viagem); //atualiza a duracao de viagem
				send_string("  ");
			}
	//vel cte:
			for(int i = 0; i < (parametros[estacao][1]-15 - tempoDeEspera*5); i++){ //tempo que mantem a velocidade constante durante a viagem.
				atraso(1, 's');
				duracao_viagem--; //diminui o tempo de duração da viagem
				tostring(str_duracao_viagem, duracao_viagem);
				proxima_linha();
				send_string(str_duracao_viagem); //atualiza o tempo de duração da viagem na tela
				send_string("  ");
			}
			limpa_reseta_cursor();
			send_string("CHEGANDO NA EST.:");
			proxima_linha();
			if(estacao+1 ==1){//verifica em qual estação está chegando
				if(ida){
					send_string("ESTACAO 2");
				}else{
					send_string("ESTACAO 1");
				}
			}else if(estacao+1 ==2){
				if(ida){
					send_string("ESTACAO 3");
				}else{
					send_string("ESTACAO 2");
				}
			}else if(estacao+1 ==3){
				if(ida){
					send_string("FIM DA LINHA");
				}else{
					send_string("ESTACAO 3");
				}
			}
			atraso(5, 's'); //atrasa 5 + 5 da rampa de descida
			for(int i=4;i>=0; i--){
				pwm_motor(vel*i);
				atraso(1, 's');
			}
			tempoDeEspera = 0;
			
			//viagem finalizada
}



void modoAuto(){
	while(1){
		if (strcmp(estado_viagem, "estacao_1") == 0){ //se está na estação 1
			//faço ele ir pra estacao 2
			limpa_reseta_cursor();
			send_string("ESTACAO ATUAL:"); //exibo a estacao atual
			proxima_linha();
			send_string("ESTACAO 1");
			ida = 1;
			realizar_viagem(1); //realizo a viagem
			strcpy(estado_viagem, "estacao_2"); //atualizo a minha parada atual na string q guarda isso
		}else if (strcmp(estado_viagem, "estacao_2") == 0){ //se está na estacao 2
			limpa_reseta_cursor();
			send_string("ESTACAO ATUAL:");
			proxima_linha();
			send_string("ESTACAO 2");
			if(ida == 1){ //se o trem esta no caminho de ida, vai p estacao 3
				realizar_viagem(2);
				strcpy(estado_viagem, "estacao_3");
			}else{ //se o trem esta no caminho de volta, vai p estacao 1
				realizar_viagem(1);
				strcpy(estado_viagem, "estacao_1");
			}
		}else if (strcmp(estado_viagem, "estacao_3") == 0){ //segue a mesma estrutura das estações anteriores
			limpa_reseta_cursor();
			send_string("ESTACAO ATUAL:");
			proxima_linha();
			send_string("ESTACAO 3");
			
			if(ida == 1){
				realizar_viagem(3);
				strcpy(estado_viagem, "fim_da_linha");
			}else{
				realizar_viagem(2);
				strcpy(estado_viagem, "estacao_2");
			}
		}else if (strcmp(estado_viagem, "fim_da_linha") == 0){ //segue a mesma estrutura das estações anteriores
			limpa_reseta_cursor();
			send_string("ESTACAO ATUAL:");
			proxima_linha();
			send_string("FIM DA LINHA");
			ida = 0; //se chegou no fim da linha, faço o caminho de volta
			realizar_viagem(3);
			strcpy(estado_viagem, "estacao_3");
			//
		}
	}
}

void modoManual(){
	while(strcmp(estado, "modoManual") == 0){
		if (strcmp(estado_viagem, "estacao_1") == 0){
			//faço ele ir pra estacao 2
			limpa_reseta_cursor();
			send_string("ESTACAO ATUAL:"); //exibo a estação atual
			proxima_linha();
			send_string("ESTACAO 1");
			ida = 1;
			atraso(1, 's');
			
			limpa_reseta_cursor();
			send_string("1-ABRE PORTAS"); //pergunto se o maquinista quer abrir as portas ou trocar o modo
			proxima_linha();
			send_string("2-MODO AUTO.");
			tecla = 0;
			while(tecla != '1' && tecla != '2'){
				lerTeclado(); //espero ele responder algo apertando 1 ou 2
			} 
			if (tecla == '1'){ //se ele abrir as portas
				tempoPortasAbertas = 0; //reseto a variavel q armazena o tempo passado com as portas abertas
				setup_PIT0_modoManual(); //esse timer faz a contabilização do tempo que se passou com as portas abertas, as interrupções vao somar no tempo
				
				limpa_reseta_cursor();
				send_string("1-FECHAR PORTAS");
				GPIOC_PSOR |= (1<<10 | 1<<11);
				tecla = 0;
				while(tecla != '1'){
					lerTeclado(); //espero o maquinista fechar as portas para seguir viagem
				}
				desliga_PIT0(); //desligo o timer q conta o tempo passado com portas abertas
				GPIOC_PCOR |= (1<<10 | 1<<11); //desligo os leds de porta aberta
			}else if(tecla == '2'){
				strcpy(estado, "modoAuto"); //se escolher trocar o modo, troco e saio da interface, voltando ao laço principal
				break;
			}
			realizar_viagem(1); //realizo a viagem
			strcpy(estado_viagem, "estacao_2"); //atualizo a estação atual
		}else if (strcmp(estado_viagem, "estacao_2") == 0){ //-----------------segue a mesma estrutura da estação anterior -------------------------------
			limpa_reseta_cursor();
			send_string("ESTACAO ATUAL:");
			proxima_linha();
			send_string("ESTACAO 2");
			atraso(1, 's');
			
			limpa_reseta_cursor();
			send_string("1-ABRE PORTAS");
			proxima_linha();
			send_string("2-MODO AUTO.");
			tecla = 0;
			while(tecla != '1' && tecla != '2'){
				lerTeclado();
			} 
			if (tecla == '1'){
				setup_PIT0_modoManual();
				tempoPortasAbertas = 0;
				limpa_reseta_cursor();
				send_string("1-FECHAR PORTAS");
				GPIOC_PSOR |= (1<<10 | 1<<11);
				tecla = 0;
				while(tecla != '1'){
					lerTeclado();
				}
				desliga_PIT0();
				GPIOC_PCOR |= (1<<10 | 1<<11);
			}else if(tecla == '2'){
				strcpy(estado, "modoAuto");
				break;
			}
			if(ida == 1){
				realizar_viagem(2);
				strcpy(estado_viagem, "estacao_3");
			}else{
				realizar_viagem(1);
				strcpy(estado_viagem, "estacao_1");
			}
		}else if (strcmp(estado_viagem, "estacao_3") == 0){ //-----------------segue a mesma estrutura da estação anterior -------------------------------
			limpa_reseta_cursor();
			send_string("ESTACAO ATUAL:");
			proxima_linha();
			send_string("ESTACAO 3");
			atraso(1, 's');
			
			limpa_reseta_cursor();
			send_string("1-ABRE PORTAS");
			proxima_linha();
			send_string("2-MODO AUTO.");
			tecla = 0;
			while(tecla != '1' && tecla != '2'){
				lerTeclado();
			} 
			if (tecla == '1'){
				setup_PIT0_modoManual();
				tempoPortasAbertas = 0;
				limpa_reseta_cursor();
				send_string("1-FECHAR PORTAS");
				GPIOC_PSOR |= (1<<10 | 1<<11);
				tecla = 0;
				while(tecla != '1'){
					lerTeclado();
				}
				desliga_PIT0();
				GPIOC_PCOR |= (1<<10 | 1<<11);
			}else if(tecla == '2'){
				strcpy(estado, "modoAuto");
				break;
			}
			if(ida == 1){
				realizar_viagem(3);
				strcpy(estado_viagem, "fim_da_linha");
			}else{
				realizar_viagem(2);
				strcpy(estado_viagem, "estacao_2");
			}
		}else if (strcmp(estado_viagem, "fim_da_linha") == 0){ //-----------------segue a mesma estrutura da estação anterior -------------------------------
			limpa_reseta_cursor();
			send_string("ESTACAO ATUAL:");
			proxima_linha();
			send_string("FIM DA LINHA");
			atraso(1, 's');
			
			limpa_reseta_cursor();
			send_string("1-ABRE PORTAS");
			proxima_linha();
			send_string("2-LOGOUT"); //se estou no fim da linha dou a opção do maquinista fazer logout ao inves de trocar o modo
			tecla = 0;
			while(tecla != '1' && tecla != '2'){
				lerTeclado();
			} 
			if (tecla == '1'){
				setup_PIT0_modoManual();
				tempoPortasAbertas = 0;
				limpa_reseta_cursor();
				send_string("1-FECHAR PORTAS");
				GPIOC_PSOR |= (1<<10 | 1<<11);
				tecla = 0;
				while(tecla != '1'){
					lerTeclado();
				}
				desliga_PIT0();
				GPIOC_PCOR |= (1<<10 | 1<<11);
			}else if(tecla == '2'){
				strcpy(estado, "inicio");
				break;
			}
			ida = 0;
			realizar_viagem(3);
			strcpy(estado_viagem, "estacao_3");
		}
	}
}

