#include <MKL25Z4.h>
#define tempoAbertura 15
//#include "timers.h"

int main(){
	
	
	double parametros[3][4] =
	{{1300, 100, 20, (60/3.6)},
	{2100, 130, 20, (70/3.6)},
	{1700, 100, 30, (63/3.6)}}; //parametros de distancia, tempo entre estacoes, tempo de parada e velocidade maxima em metros por segundo.

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

	PORTC_PCR7 = 1<< 8;//habilitar GPIO de todos os pinos usados no teclado
	PORTC_PCR0 = 1<< 8;
	PORTC_PCR3 = 1<< 8;
	PORTC_PCR4 = 1<< 8;
	PORTA_PCR1 = ((1<< 8) + 3) | (10 << 16);//habilitar pull-up dos pinos da coluna do teclado
	PORTA_PCR2 = ((1<< 8) + 3) | (10 << 16);
	PORTA_PCR4 = ((1<< 8) + 3) | (10 << 16);
	PORTA_PCR5 = ((1<< 8) + 3) | (10 << 16);

	SIM_SOPT2 |= 1<<24; //habilita clock dos timers
	SIM_SCGC6 |= 1<< 26; //habilita timer 2
	
	//pinos cooler
	
	//PORTA_PCR12 = ((1<< 8) + 3) | (10 << 16);//habilitar interrupcao pelo tequimetro
	//GPIOE_PDDR |= 1 << 30;
	//PORTE_PCR30 = (3 << 8) + 3;

	config_lcd_padrao();
	limpa_reseta_cursor();
	telaArmMetro();

	__enable_irq();
			
	NVIC_EnableIRQ(PORTA_IRQn);
	
	
	configura_motor();
	
			
	
	while(1)
	{
		pwm_motor(0);
		atraso(3, 's');
		pwm_motor(20);
		atraso(3, 's');
		pwm_motor(40);
		atraso(3, 's');
		pwm_motor(60);
		atraso(3, 's');
		pwm_motor(80);
		atraso(3, 's');
		pwm_motor(60);
		atraso(3, 's');
		pwm_motor(40);
		atraso(3, 's');
		pwm_motor(20);
		atraso(3, 's');
		pwm_motor(0);
		atraso(3, 's');
	/*	calibracaoDeRotseg();
		

		
		while(distancia > 0)
		{*/

				//GPIOE_PTOR = 1 << 30;
				//velocidade(80);
	
		/*}
		
		interrupcao()
		{
			tempototal --;
		}
		 interrupcao(){
				debounce();
				distancia-22.22/rotSeg;
		 }*/
	}
}

void PORTA_IRQHandler(void){
	send_data(procuraTecla());
	PORTA_PCR1 |= (1 << 24);
	PORTA_PCR2 |= (1 << 24);
	PORTA_PCR4 |= (1 << 24);
	PORTA_PCR5 |= (1 << 24);
}

	