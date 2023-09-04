/*GRUPO A - Gabriel Sadigursky, Vinicius Zortea, Vitor Simon.

Esse é o código do metro que à principio é pra funcionar tudo menos o controle proporcional da velocidade do motor.

*/
#include <MKL25Z4.h>
#include <string.h>
#include <stdlib.h>
#include "menus.h"
#include "rotinas_viagem.h"
#include "controle_motor.h"
#include "timers.h"
#include "lcdio.h"
#include "teclado.h"


int contagemBlinkLed = 0;
char strtempoPortasAbertas[10];

int main(){
	SIM_SCGC5 |= (1 << 11);// habilitar PORTC
	SIM_SCGC5 |= (1 << 12);// habilitar PORTD
	SIM_SCGC5 |= (1 << 9);// habilitar PORTA
  SIM_SCGC5 |= (1 << 13);// habilitar PORTE


	GPIOC_PDDR |= (1 << 7 | 1 << 0 | 1 << 3 | 1 << 4);//pino linha  do teclado como output
	GPIOA_PDDR &= ~(1 << 1 | 1 << 2 | 1 << 4 | 1 << 5);//pino colunado teclado como input
	GPIOD_PDDR = 0b11111111;//pinos de D0-D7 do LCD como output
	GPIOC_PDDR |= (1 << 8 | 1 << 9);// pinos do RS e Enable do teclado como output

	PORTD_PCR0 = 1<< 8; //habilitar GPIO de todos os pinos usados no LCD
	PORTD_PCR1 = 1<< 8;
	PORTD_PCR2= 1<< 8;
	PORTD_PCR3= 1<< 8;
	PORTD_PCR4 = 1<< 8;
	PORTD_PCR5 = 1<< 8;
	PORTD_PCR6 = 1<< 8;
	PORTD_PCR7 = 1<< 8;
	PORTC_PCR9 = 1<< 8;
	PORTC_PCR8 = 1<< 8;
	//Pinos Led
	PORTC_PCR10 = 1<< 8;
	PORTC_PCR11 = 1<< 8;
	GPIOC_PDDR |= (1<<10 | 1<<11);
	

	PORTC_PCR7 = 1<< 8;//habilitar GPIO de todos os pinos usados no teclado
	PORTC_PCR0 = 1<< 8;
	PORTC_PCR3 = 1<< 8;
	PORTC_PCR4 = 1<< 8;
	PORTA_PCR1 = ((1<< 8) + 3);//habilitar pull-up dos pinos da coluna do teclado
	PORTA_PCR2 = ((1<< 8) + 3);
	PORTA_PCR4 = ((1<< 8) + 3);
	PORTA_PCR5 = ((1<< 8) + 3);
	
	SIM_SOPT2 |= 1<<24; //habilita clock dos timers
	SIM_SCGC6 |= 1<< 26; //habilita timer 2
	SIM_SCGC6 |= 1<< 24;//habilitar timer 0
	
	//pinos cooler
	//GPIOA_PDDR &= ~(1 << 12);//pino tacometro como iput     //nesse programa não estamos usando.
	//PORTA_PCR12 = ((1<< 8) ) | (10 << 16);    
	
	NVIC_EnableIRQ(PORTA_IRQn); //ja deixo habilitado a interrupção da porta A de antemão, dps só mexemos no registrador p alterar qnd queremos interrupção ou não
	
	config_lcd_padrao();
	limpa_reseta_cursor();
	
	configura_motor(); //configuro os pinos e timer responsaveis pela ativação do motor
	
		
	while(1)
	{
		if (strcmp(estado, "inicio") == 0){ //pede senha pro maquinista se logar
			pwm_motor(0);
			telaArmMetro();
			lerSenha();
		}else if (strcmp(estado, "logado") == 0){ //se esta logado como maquinista, entra na rotina de seleção de modo
			selecaoModoMaquinista();
		}else if (strcmp(estado, "adm") == 0){ //se esta logado como adm, entra na edição de parametros
			edicaoParametros();
		}else if (strcmp(estado, "modoAuto") == 0){ //se o maquinista seleciona o modo automatico, entra na interface de modo automatico
			modoAuto();
		}else if (strcmp(estado, "modoManual") == 0){  //se o maquinista seleciona o modo manual, entra na interface de modo manual
			modoManual();
		}
	}
}

void PIT_IRQHandler(){
if(strcmp(estado, "modoAuto")==0){
		if(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK){ //ve se foi o canal 0 q estourou
			PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK; //limpa flag
			setup_PIT1(); //se estourou o canal 0 quer dizer q se passaram 12s, ai faço os leds blinkarem usando o canal 1
			//desligar pit0
			desliga_PIT0(); //desligo o canal 0
			PIT->CHANNEL[0].LDVAL = 0;
		}
		if(PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK){ //ve se foi o canal 1 q estourou //tem q estourar 12 vezes
				if (contagemBlinkLed < 12){ //faço o blink dos leds no canal 1
					PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK;
					//reseta pit 1
					contagemBlinkLed++;
					//dou toggle na porta do led
					GPIOC_PTOR |= (1<<10 | 1<<11);
				}else if (contagemBlinkLed >= 12){ //se ja blinkei 12x, desligo td e fecho as portas
					PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK; 
					//desligo pit 1
					desliga_PIT1();
					GPIOC_PCOR |= (1<<10 | 1<<11);
					contagemBlinkLed = 0;
					portas_abertas = 0;
					PORTA_PCR5 &= ~(10 << 16); //desabilito interrupcao da porta A5
					//fim do tempo, portas se fecharao
				}
		}
	}else if(strcmp(estado, "modoManual")==0){ //no modo manual, o canal 0 cuida o tempo que se passou com as portas abertas estourando 1x por segundo e atualizando o tempo no display e na variavel
		if(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK){ //ve se foi o canal 0 q estourou
			PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
			tostring(strtempoPortasAbertas,tempoPortasAbertas);
			proxima_linha();
			send_string(strtempoPortasAbertas); //printa o tempo passado com as portas abertas
			tempoPortasAbertas++;
			send_command(0x80); //lugar equivalente a posicao zero do lcd
			atraso(3, 'm');
		}
	}
}

void PORTA_IRQHandler(void){
	PORTA_PCR5 &= ~(10 << 16); //desativa interrupção enquanto estamos tratando ela
	tecla = procuraTecla();
	if(tecla == 'A' || tecla == 'B'){ //se A ou B foram apertados com a interrupção ativa, é adicionado mais 5s de porta aberta
		if((GPIOA_PDIR & (1 << 5)) == 0){
			tempoDeEspera++;
				if(PIT->CHANNEL[0].CVAL != 0){
				desliga_PIT0();
				PIT->CHANNEL[0].LDVAL = PIT->CHANNEL[0].LDVAL + 52428800; //somo 5s
				//ligo o PIT 0 dnv
				PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
				PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
				
			}else if(PIT->CHANNEL[0].CVAL ==0){

				//pit0 ==tempo no led + 2s 
				//pit1 == 3s
				desliga_PIT1();
				desliga_PIT0();
				PIT->CHANNEL[0].LDVAL = PIT->CHANNEL[1].CVAL + contagemBlinkLed*2621440 + 2*10485760;
				PIT->CHANNEL[1].LDVAL = 2621440; //valor para 0.25s, fazendo contagem =0 novamente temos o timer de 3s resetado
				contagemBlinkLed = 0;
				//ligo o PIT 0 dnv
				PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
				PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
				GPIOC_PSOR |= (1<<10 | 1<<11);
			}
		}
	}
	
	PORTA_PCR5 |= (1 << 24); //reseta flag da interrupção
	PORTA_PCR5 |= (10 << 16); //ativa interrupção na coluna dos botoes da porta, teclas A e B
}