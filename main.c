#include <MKL25Z4.h>
#include <string.h>
#include "controle_motor.h"
#include "timers.h"
#include "lcdio.h"
#include "teclado.h"
#include "validacao_senhas.h"

#define tempoAbertura 15




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
	//NVIC_EnableIRQ(PORTA_IRQn);

	config_lcd_padrao();
	limpa_reseta_cursor();
	
		
	while(1)
	{
		if (strcmp(estado, "inicio") == 0){ //pede senha pro maquinista se logar
			telaArmMetro();
			lerSenha();
		}else if (strcmp(estado, "logado") == 0){
			// do something else
		}else if (strcmp(estado, "adm") == 0){
			edicaoParametros();
		}else if (strcmp(estado, "editvel") == 0){
			edicaoParametros();
		}
		
		
		
		else{
		}
		send_data(tecla);
		atraso(3, 's');
	}
}

/*void PORTA_IRQHandler(void){
	
	tecla = procuraTecla();
	send_data(tecla);
	PORTA_PCR1 |= (1 << 24);
	PORTA_PCR2 |= (1 << 24);
	PORTA_PCR4 |= (1 << 24);
	PORTA_PCR5 |= (1 << 24);
}*/

	