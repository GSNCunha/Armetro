#include <MKL25Z4.h>
#define frequencia_placa (8/3)*8000000 //é 21,333Mhz

int pulsos_80km;
int pulsos_70km;
int pulsos_63km;
int pulsos_60km;

int tempoPorPulso_80km;
int tempoPorPulso_70km;
int tempoPorPulso_63km;
int tempoPorPulso_60km;

void configura_motor(){
	SIM_SCGC5 |= (1 << 10);// habilitar PORTB
	SIM_SCGC6 |= (1 << 25); //habilita timer 1
	//pino 0 porta B //pino do pwm do cooler
	GPIOB_PDDR |= 1<< 0;
	PORTB_PCR0 = 3<< 8; //coloca o pino 2 conectado no timer 2 com a opção de mux, para ver isso é na pagina 163 do datasheet
}

void pwm_motor(int velocidade_desejada){ //este timer está conectado ao pino 0 da porta B, nao mexer
	float duty_cycle_motor;
	duty_cycle_motor = velocidade_desejada*75/80+25;
	
	TPM1_SC = (1 << 3) + 0; //ativa incremento no timer, sem prescaler
	TPM1_MOD = 853; //periodo total
	TPM1_C0SC = (2 << 4) + (2 << 2); // High-true pulses (clear Output on match, set Output on reload)
	TPM1_C0V = (int)duty_cycle_motor*853/100; //quanto a gnt quer do periodo
	//duty_cycle_motor é a porcentagem do pwm, 0 = 0%, 100 = 100% da potencia
}

void calibra_motor(){
	pwm_motor(100);
	atraso(5, 's');
	pulsos_80km = ler_pulsos_motor(); //numero de pulsos por segundo à 80km/h
	pulsos_70km = pulsos_80km*70/80;	//numero de pulsos por segundo à 70km/h
	pulsos_63km = pulsos_80km*63/80;  //numero de pulsos por segundo à 63km/h
	pulsos_60km = pulsos_80km*60/80;  //numero de pulsos por segundo à 60km/h
	tempoPorPulso_80km = 1/pulsos_80km; //tempo entre pulsos à 80km/h
	tempoPorPulso_70km = 1/pulsos_70km;	//tempo entre pulsos à 70km/h
	tempoPorPulso_63km = 1/pulsos_63km;	//tempo entre pulsos à 63km/h
	tempoPorPulso_60km = 1/pulsos_60km; //tempo entre pulsos à 60km/h
	pwm_motor(25);
	atraso(5, 's'); 
	//calibragem concluida, agr funciona normal
	/*se eu to a 80km/h eu tenho X pulsos/segundo
		80km/h = pulsos_80km/s
		60km/h = pulsos_60km/s
	*/
}

int ler_pulsos_motor(){//função que lê quantos pulsos se passaram em 1s
	//2 pulsos por rotação
	//60km/h = 16,667 m/s
	//63km/h = 17,5 m/s
	//70km/h = 19,44 m/s
	//80km/h = 22,22 m/s
}

int pulsosParaVel(){
	
}


void atualiza_vel_motor(){

}

