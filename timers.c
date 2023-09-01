#include <MKL25Z4.h>
#include "timers.h"
#include "lcdio.h"
#define periodo 853//((8*8000000/3)/25000) //calculo periodo para frequencia de 25000 hz

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
	
		//Com o prescale de 64, o delay de 1 segundo ocorrera apos 5 overflows do timer
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
//porcentagem de periodo eq   uivalente (80km/h --> 0*periodo, 24KM/h(min) --> 70%*periodo)
void velocidade(char velocidade){
	TPM2_SC = (1 << 3);
	TPM2_MOD = periodo;//periodo total
	TPM2_C0SC = (2 << 4) + (2 << 2); // Clear pin on match
	TPM2_C0V = periodo/3;  
	
	while((TPM2_SC & (1 << 7)) == 0);
	TPM2_SC |= 1<<7;

}


void setup_PIT0(){// delay porta
	SIM_SCGC6 |= 1<<23; //ativa PIT
	PIT_MCR = 0x00; // 0 aqui pra habilitar o clock
	PIT_LDVAL0 = 125829120; //valor inicial pro timer, ele conta até 0 e gera interrupção //12segundos = 125829120
	//valor atual = PIT_CVAL0;
	PIT_TCTRL0 = 1 << 30 | 1 << 31; // 1<<30 ativa interrupcao, 1<<31 ativa o timer
	NVIC_SetPriority(PIT_IRQn, 3); 
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_EnableIRQ(PIT_IRQn);
	send_data('c');
}
void setup_PIT1(){// led
	SIM_SCGC6 |= 1<<23; //ativa PIT
	PIT_MCR = 0x00; // 0 aqui pra habilitar o clock
	PIT_LDVAL1 = 2621440; //valor inicial pro timer, ele conta até 0 e gera interrupção //3segundos = 31457280
	//valor atual = PIT_CVAL1;
	PIT_TCTRL1 = 1 << 30 | 1 << 31; // 1<<30 ativa interrupcao, 1<<31 ativa o timer
	NVIC_SetPriority(PIT_IRQn, 3); 
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_EnableIRQ(PIT_IRQn);
}

void desliga_PIT1(){
	PIT_TCTRL1 = 0;
}
void desliga_PIT0(){
	PIT_TCTRL0 = 0;
}

void soma5_PIT0(){
	

}

