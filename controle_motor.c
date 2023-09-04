#include <MKL25Z4.h>
#include "controle_motor.h"
#include "timers.h"
#include "lcdio.h"

#define frequencia_placa (8/3)*8000000 //é 21,333Mhz

float velocidadeAtual;

void configura_motor(){
	SIM_SCGC5 |= (1 << 10);// habilitar PORTB
	SIM_SCGC6 |= (1 << 25); //habilita timer 1
	//pino 0 porta B //pino do pwm do cooler
	GPIOB_PDDR |= 1<< 0;
	PORTB_PCR0 = 3<< 8; //coloca o pino 2 conectado no timer 1 com a opção de mux, para ver isso é na pagina 163 do datasheet
}

void pwm_motor(float velocidade_desejada){ //este timer está conectado ao pino 0 da porta B, nao mexer
	float duty_cycle_motor;
	duty_cycle_motor = velocidade_desejada*75/80+25; //duty cycle considerando os limites de 0 à 80km/h com 0km/h sendo 25%
	TPM1_SC = (1 << 3) + 0; //ativa incremento no timer, sem prescaler
	TPM1_MOD = 853; //periodo total
	TPM1_C0SC = (2 << 4) + (2 << 2); // High-true pulses (clear Output on match, set Output on reload)
	TPM1_C0V = (int)duty_cycle_motor*853/100; //quanto a gnt quer do periodo
	//duty_cycle_motor é a porcentagem do pwm, 0 = 0%, 100 = 100% da potencia
	velocidadeAtual = velocidade_desejada;
}