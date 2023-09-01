#include <MKL25Z4.h>
#include "controle_motor.h"
#include "timers.h"
#include "lcdio.h"

#define frequencia_placa (8/3)*8000000 //é 21,333Mhz
/*static float pulsos = 0;
static float rotacoes_80kmh = 0;
static float rotacoes_0kmh = 0;
static int motor_calibrado = 0;
static float velocidadeAtual = 0;
*/
float distancia;
float tempoFaltando;

float tempoEntrePulsos;

float pulsos;
float rotacoes_80kmh;
float rotacoes_0kmh;
int motor_calibrado;
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
	duty_cycle_motor = velocidade_desejada*75/80+25;
	
	TPM1_SC = (1 << 3) + 0; //ativa incremento no timer, sem prescaler
	TPM1_MOD = 853; //periodo total
	TPM1_C0SC = (2 << 4) + (2 << 2); // High-true pulses (clear Output on match, set Output on reload)
	TPM1_C0V = (int)duty_cycle_motor*853/100; //quanto a gnt quer do periodo
	//duty_cycle_motor é a porcentagem do pwm, 0 = 0%, 100 = 100% da potencia
	velocidadeAtual = velocidade_desejada;
}

void calibra_motor(){

	
	pwm_motor(80);
	atraso(3, 's');
	PORTA_PCR12 = (1<< 8) | (10 << 16);//liga interrupcao
	pulsos = 0;
	atraso(5, 's');
	PORTA_PCR12 = (1<< 8);//desliga interrupcao
	rotacoes_80kmh = (pulsos/2)*6*60; //em rotacoes/h
	send_data(rotacoes_80kmh);
	pwm_motor(0);
	atraso(3, 's');
	PORTA_PCR12 = (1<< 8) | (10 << 16);//liga interrupcao
	atraso(5, 's');
	pulsos = 0;
	PORTA_PCR12 = ((1<< 8) + 3);//desliga interrupcao
	rotacoes_0kmh = (pulsos/2)*6*60; //em rotacoes/h
	send_data(rotacoes_0kmh);
	//---------------
	pulsos = 0;
	motor_calibrado = 1;
	pwm_motor(0);
	send_string("calibrado");
	atraso(1,'s');
	PORTA_PCR12 = (1<< 8) | (10 << 16);//liga interrupcao
	
}


float km_hParaGiros_s(float km_h){ //apos calibrar o motor, sempre que usar essa função, dado uma velocidade em km/h será retornada uma outra velocidade em giros/s
	float giros_h = 0;
	float giros_s = 0;
	
	giros_h = ((rotacoes_80kmh-rotacoes_0kmh)/(80-0))*km_h + rotacoes_0kmh;
	giros_s = giros_h /(60*60);
	
	return giros_s;
}

float km_hParaGiros_h(float km_h){ //apos calibrar o motor, sempre que usar essa função, dado uma velocidade em km/h será retornada uma outra velocidade em giros/s
	float giros_h = 0;
	float giros_s = 0;
	
	giros_h = ((rotacoes_80kmh-rotacoes_0kmh)/(80-0))*km_h + rotacoes_0kmh;
	return giros_h;
}

void RTC_timer_configura(){
	SIM_SCGC6 |= 1 << 29; //habilita o RTC
	RTC_CR |= 1<< 8; //ativa o oscilador
	RTC_SR = 0; //desabilita o time counter p poder escrever no timer
	RTC_TPR = 0;
	RTC_TSR = 0;
	RTC_SR |= 1 << 4; //habilita o time counter
}

void RTC_timer_reseta(){
	RTC_SR = 0; //desabilita o time counter p poder escrever no timer
	RTC_TPR = 0; //incrementa com frequencia de 32,768 kHz
	RTC_TSR = 0; //incrementa a cada segundo
	RTC_SR |= 1 << 4; //habilita o time counter
}
