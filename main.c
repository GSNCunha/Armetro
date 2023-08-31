#include <MKL25Z4.h>

#include "controle_motor.h"
#include "timers.h"
#include "lcdio.h"

#define tempoAbertura 15
/*
volatile static float distancia;
volatile static float tempoFaltando;

volatile static float tempoEntrePulsos;

volatile static float pulsos;
volatile static float rotacoes_80kmh;
volatile static float rotacoes_0kmh;
volatile static int motor_calibrado;
volatile static float velocidadeAtual;
*/


int main(){
	distancia = 1300;
	tempoFaltando = 20;
	tempoEntrePulsos = 0;
	pulsos = 0;
	rotacoes_80kmh = 0;
	rotacoes_0kmh = 0;
	motor_calibrado = 0;
	velocidadeAtual = 0;
	
	double parametros[3][4] =
	{{1300, 100, 20, (60/3.6)},
	{2100, 130, 20, (70/3.6)},
	{1700, 100, 30, (63/3.6)}}; //parametros de distancia, tempo entre estacoes, tempo de parada e velocidade maxima em metros por segundo.

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

	//PORTC_PCR7 = 1<< 8;//habilitar GPIO de todos os pinos usados no teclado
	//PORTC_PCR0 = 1<< 8;
	//PORTC_PCR3 = 1<< 8;
	//PORTC_PCR4 = 1<< 8;
	//PORTA_PCR1 = ((1<< 8) + 3) | (10 << 16);//habilitar pull-up dos pinos da coluna do teclado einterrupcao
	//PORTA_PCR2 = ((1<< 8) + 3) | (10 << 16);
	//PORTA_PCR4 = ((1<< 8) + 3) | (10 << 16);
	//PORTA_PCR5 = ((1<< 8) + 3) | (10 << 16);
	
	SIM_SOPT2 |= 1<<24; //habilita clock dos timers
	SIM_SCGC6 |= 1<< 26; //habilita timer 2
	SIM_SCGC6 |= 1<< 24;//habilitar timer 0
	
	//pinos cooler
	GPIOA_PDDR &= ~(1 << 12);//pino tacometro como iput
	PORTA_PCR12 = ((1<< 8) ) | (10 << 16);//habilitar interrupcao pelo tacometro //+3 pra ativar pullup, tirei
	
	__enable_irq();
	NVIC_EnableIRQ(PORTA_IRQn);

	config_lcd_padrao();
	limpa_reseta_cursor();
	telaArmMetro();
	
	

	
	
	configura_motor();
	calibra_motor();

	
	
	//sequencia motor
	motor_calibrado = 1;
	
			
	while(1)
	{
		pwm_motor(velocidadeAtual);
	}
}

void PORTA_IRQHandler(void){
	PORTA_PCR12 = (1<< 8);//desliga interrupcao
	pulsos++;
	
	if (motor_calibrado == 1){	//se o motor ja tiver calibrado, queremos ficar lendo o tacometro e vendo se a velocidade colocada corresponde corretamente com a leitura do sensor.
		//diminuir distancia necessaria pra chegar no final:
		send_data('a');
		if(velocidadeAtual != 0){
			float metrosPorPulso;
			metrosPorPulso = (velocidadeAtual/km_hParaGiros_h(velocidadeAtual))*(1000/2);
			distancia = distancia - metrosPorPulso;
		}
		
		//conferencia dos pulsos do sensor:
		tempoEntrePulsos = RTC_TPR*1/32768;
		RTC_timer_reseta();
		tempoFaltando = tempoFaltando - tempoEntrePulsos;
		//send_data(tempoFaltando);
		//------------------ 
		//conferir velocidade
		//recalcular a velocidade com base distancia que falta e o tempo certo pra chegar
		float segPorPulso = (1/km_hParaGiros_s(velocidadeAtual))/2;
			//ajustar conforme distancia e tempo faltando
			if (distancia/tempoFaltando < 80){
				pwm_motor(distancia/tempoFaltando);
			} //dps ajustar, cada trecho tem uma velocidade maxima
			else{
				pwm_motor(80);
			}
			
			if(distancia <= 0 || tempoFaltando <= 0){
				send_string("fim");
			}
			
	}	
	//atrasoTimerZero(20, 'm');
	//PORTA_PCR1 |= (1 << 24);
	//PORTA_PCR2 |= (1 << 24);
	//PORTA_PCR4 |= (1 << 24);
	//PORTA_PCR5 |= (1 << 24);
	PORTA_PCR12 |= (1 << 24); //limpa flag
	PORTA_PCR12 |= (1<< 8) | (10 << 16);// liga interrupcao //1<<8 p usar como pino, +3 pra ativar pullup, 10<<16 pra interrupção na borda de descida.
}

	