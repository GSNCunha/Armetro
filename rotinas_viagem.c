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

char estado_viagem[16] = "estacao_1";
int tempoPortasAbertas = 0;
double parametros[3][4] =
	{{1300, 30, 20, 60},
	{2100, 30, 20, 70},
	{1700, 30, 30, 63}}; //parametros de distancia, tempo entre estacoes, tempo de parada e velocidade maxima em km/h.
	
int portas_abertas = 0;
int tempoDeEspera = 0;
int ida = 1;

void abrePortasEspera(){
	PORTA_PCR5 |= ((1<< 8) + 3) | (10 << 16); //ativa interrupção na coluna dos botoes da porta, teclas A e B
	portas_abertas=1;
	if(portas_abertas == 1){
		setup_PIT0();
		GPIOC_PSOR |= (1<<10 | 1<<11);
	}
	while(portas_abertas==1);
	GPIOC_PCOR |= (1<<10 | 1<<11);
	PORTA_PCR5 &= ~(10 << 16);
}

void tostring(char str[], int num)
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
			estacao--;
		
	if(strcmp(estado, "modoAuto") == 0)
	{
		abrePortasEspera();
	
	}
			atraso(parametros[estacao][2]-15, 's');
			float vel = (parametros[estacao][0])/((parametros[estacao][1])-5);
			int duracao_viagem = parametros[estacao][1] - tempoDeEspera*5;
			char str_duracao_viagem [10];
			
	//rampa subida:
			limpa_reseta_cursor();
			send_string("DURACAO VIAGEM:");
			for(int i=1;i <6; i++){
				pwm_motor(vel*i);
				atraso(1, 's');
				duracao_viagem--;
				tostring(str_duracao_viagem, duracao_viagem);
				proxima_linha();
				send_string(str_duracao_viagem);
				send_string("  ");
			}
	//vel cte:
			for(int i = 0; i < (parametros[estacao][1]-15 - tempoDeEspera*5); i++){
				atraso(1, 's');
				duracao_viagem--;
				tostring(str_duracao_viagem, duracao_viagem);
				proxima_linha();
				send_string(str_duracao_viagem);
				send_string("  ");
			}
			limpa_reseta_cursor();
			send_string("CHEGANDO NA EST.:");
			proxima_linha();
			if(estacao+1 ==1){
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
			atraso(5, 's');
			for(int i=4;i>=0; i--){
				pwm_motor(vel*i);
				atraso(1, 's');
			}
			tempoDeEspera = 0;
}



void modoAuto(){
	//strcpy(estado_viagem, "estacao_1"); 
	//int ida = 1;
	while(1){
		if (strcmp(estado_viagem, "estacao_1") == 0){ //pede senha pro maquinista se logar
			//faço ele ir pra parada 1
			limpa_reseta_cursor();
			send_string("ESTACAO ATUAL:");
			proxima_linha();
			send_string("ESTACAO 1");
			ida = 1;
			realizar_viagem(1);
			strcpy(estado_viagem, "estacao_2");
		}else if (strcmp(estado_viagem, "estacao_2") == 0){
			//faço ele ir pra parada 2
			limpa_reseta_cursor();
			send_string("ESTACAO ATUAL:");
			proxima_linha();
			send_string("ESTACAO 2");
			if(ida == 1){
				realizar_viagem(2);
				strcpy(estado_viagem, "estacao_3");
			}else{
				realizar_viagem(1);
				strcpy(estado_viagem, "estacao_1");
			}
		}else if (strcmp(estado_viagem, "estacao_3") == 0){
			//faço ele ir pra parada 3
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
		}else if (strcmp(estado_viagem, "fim_da_linha") == 0){
			limpa_reseta_cursor();
			send_string("ESTACAO ATUAL:");
			proxima_linha();
			send_string("FIM DA LINHA");
			ida = 0;
			realizar_viagem(3);
			strcpy(estado_viagem, "estacao_3");
			//
		}
	}
}

void modoManual(){
	while(strcmp(estado, "modoManual") == 0){
		if (strcmp(estado_viagem, "estacao_1") == 0){
			//faço ele ir pra parada 1
			limpa_reseta_cursor();
			send_string("ESTACAO ATUAL:");
			proxima_linha();
			send_string("ESTACAO 1");
			ida = 1;
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
			realizar_viagem(1);
			strcpy(estado_viagem, "estacao_2");
		}else if (strcmp(estado_viagem, "estacao_2") == 0){
			//faço ele ir pra parada 2
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
		}else if (strcmp(estado_viagem, "estacao_3") == 0){
			//faço ele ir pra parada 3
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
		}else if (strcmp(estado_viagem, "fim_da_linha") == 0){
			limpa_reseta_cursor();
			send_string("ESTACAO ATUAL:");
			proxima_linha();
			send_string("FIM DA LINHA");
			atraso(1, 's');
			
			limpa_reseta_cursor();
			send_string("1-ABRE PORTAS");
			proxima_linha();
			send_string("2-LOGOUT");
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

