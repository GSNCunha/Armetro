#include <MKL25Z4.h>
#include <string.h>
#include "controle_motor.h"
#include "timers.h"
#include "lcdio.h"
#include "teclado.h"
#include "validacao_senhas.h"

int contagemBlinkLed = 0;

int main(){
	distancia = 1300;
	tempoFaltando = 20;
	tempoEntrePulsos = 0;
	pulsos = 0;
	rotacoes_80kmh = 0;
	rotacoes_0kmh = 0;
	motor_calibrado = 0;
	velocidadeAtual = 0;
	
	
	SIM_SCGC5 |= (1 << 11);// habilitar PORTC
	SIM_SCGC5 |= (1 << 12);// habilitar PORTD
	SIM_SCGC5 |= (1 << 9);// habilitar PORTA
  SIM_SCGC5 |= (1 << 13);// habilitar PORTE
	RTC_timer_configura();


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
	PORTA_PCR1 = ((1<< 8) + 3);// | (10 << 16);//habilitar pull-up dos pinos da coluna do teclado einterrupcao
	PORTA_PCR2 = ((1<< 8) + 3);// | (10 << 16);
	PORTA_PCR4 = ((1<< 8) + 3);// | (10 << 16);
	PORTA_PCR5 = ((1<< 8) + 3);// | (10 << 16);
	
	SIM_SOPT2 |= 1<<24; //habilita clock dos timers
	SIM_SCGC6 |= 1<< 26; //habilita timer 2
	SIM_SCGC6 |= 1<< 24;//habilitar timer 0
	
	//pinos cooler
	GPIOA_PDDR &= ~(1 << 12);//pino tacometro como iput
	PORTA_PCR12 = ((1<< 8) ) | (10 << 16);//habilitar interrupcao pelo tacometro //+3 pra ativar pullup, tirei
	
	//__enable_irq();
	NVIC_SetPriority(PORTA_IRQn, 0);
	NVIC_EnableIRQ(PORTA_IRQn);

	config_lcd_padrao();
	limpa_reseta_cursor();
	
	configura_motor();
	
		
	while(1)
	{
		if (strcmp(estado, "inicio") == 0){ //pede senha pro maquinista se logar
			pwm_motor(0);
			telaArmMetro();
			lerSenha();
		}else if (strcmp(estado, "logado") == 0){
			selecaoModoMaquinista();
		}else if (strcmp(estado, "adm") == 0){
			edicaoParametros();
		}else if (strcmp(estado, "modoAuto") == 0){
			modoAuto();
		}else if (strcmp(estado, "modoManual") == 0){
			//
		}
	}
}

void PIT_IRQHandler(){
	if(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK){ //ve se foi o canal 0 q estourou
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
		setup_PIT1();
		//desligar pit0
		desliga_PIT0();
		PIT->CHANNEL[0].LDVAL = 0;
	}
	if(PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK){ //ve se foi o canal 1 q estourou //tem q estourar 12 vezes
			if (contagemBlinkLed < 12){
				PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK;
				//reseta pit 1
				//PIT->CHANNEL[1].TFLG = 1<<31;
				contagemBlinkLed++;
				//dou toggle na porta do led
				GPIOC_PTOR |= (1<<10 | 1<<11);
			}else if (contagemBlinkLed >= 12){
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
	
}

void PORTA_IRQHandler(void){
	atraso(5, 'm');
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
	PORTA_PCR5 |= (1 << 24); //reseta flag da interrupção
}