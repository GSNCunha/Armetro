#include <MKL25Z4.h>
#include "lcdio.h"
#define tempoAbertura 15



int main(){
	
	double parametros[3][4] =
{{1300, 100, 20, (60/3.6)},
{2100, 130, 20, (70/3.6)},
{1700, 100, 30, (63/3.6)}}; //parametros de distancia, tempo entre estacoes, tempo de parada e velocidade maxima em metros por segundo.


	SIM_SCGC5 |= (1 << 11);// habilitar PORTC
	SIM_SCGC5 |= (1 << 12);// habilitar PORTD
	SIM_SCGC5 |= (1 << 9);// habilitar PORTA

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
	PORTA_PCR1 = ((1<< 8) + 3);//habilitar pull-up dos pinos da coluna do teclado
	PORTA_PCR2 = ((1<< 8) + 3);
	PORTA_PCR4 = ((1<< 8) + 3);
	PORTA_PCR5 = ((1<< 8) + 3);

	SIM_SOPT2 |= 1<<24; //habilita clock dos timers
	SIM_SCGC6 |= 1<< 26 | 1 << 25; //habilita timer 2

	config_lcd_padrao();
	limpa_reseta_cursor();
	telaArmMetro();
	while(1)
	{
		digitarSenha();
	}
}
	