#include <MKL25Z4.h>
#include "timers.h"
#define periodo ((8*8000000/3)/25000) //calculo periodo para frequencia de 25000 hz

void delay_s(int segundos){
	
	TPM2_SC = (1<<7) + (1<<3) + 6;
	TPM2_MOD = 0XFFFF;
	
	//Com o prescale de 64, o delay de 1 segundo ocorrerá após 5 overflows do timer
	TPM2_SC |= (1<<7);
	int ciclos = 0;
	
	while(ciclos < (segundos*5)){
		while((TPM2_SC & (1<<7)) == 0);
		TPM2_SC |= (1<<7);
		ciclos++;
	}
}

void delay_ms(int miliSegundos){
	
	TPM2_SC = (1<<7) + (1<<3) + 6;
	TPM2_MOD = (8*8000000/3)*(miliSegundos/1000)/64;
	
		//Com o prescale de 64, o delay de 1 segundo ocorrerá após 5 overflows do timer
	TPM2_SC |= (1<<7);
	
	while((TPM2_SC & (1<<7)) == 0);
	TPM2_SC |= (1<<7);
}

void atraso(int num, char param)
{
	if(param == 's')
		delay_s(num);
	else if( param == 'm')
		delay_ms(num);
}

//funcao linear que recebe a velocidade em quilometros por hora e retorna a 
//porcentagem de periodo equivalente (80km/h --> 0*período, 24KM/h(min) --> 70%*periodo)
void velocidade(char velocidade){
	TPM2_SC = (1 << 3) + 0;
	TPM2_MOD = periodo;//periodo total
	TPM2_C0SC = (2 << 4) + (2 << 2); // Clear pin on match
	TPM2_C0V = periodo*(-0.0125*velocidade + 1); 
}
