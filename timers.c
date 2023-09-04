#include <MKL25Z4.h>
#include "timers.h"
#include "lcdio.h"

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
	TPM2_MOD = miliSegundos*5*65535/1000;
	
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

void setup_PIT0(){// delay porta
	
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; //habilita o modulo pit
	PIT->MCR &= ~PIT_MCR_MDIS_MASK; //habilita o timer
	PIT->CHANNEL[0].LDVAL =125829120;   // tempo dos 12s = 12*10485760
	PIT->CHANNEL[0].TCTRL &= PIT_TCTRL_CHN_MASK; //nao encadeia os timers
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK; //habilita o contador
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK; //ativa interrupção
	NVIC_SetPriority(PIT_IRQn, 3);
	NVIC_ClearPendingIRQ(PIT_IRQn); //limpa qlqr interrupção pendente do pit 
	NVIC_EnableIRQ(PIT_IRQn);	//habilita interrupção
}
void setup_PIT1(){// led
	
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; //habilita o modulo pit
	PIT->MCR &= ~PIT_MCR_MDIS_MASK; //habilita o timer
	PIT->CHANNEL[1].LDVAL =2621440;   //timer do blink == 0.25*10485760
	PIT->CHANNEL[1].TCTRL &= PIT_TCTRL_CHN_MASK; //nao encadeia os timers
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK; //habilita o contador
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TIE_MASK;  //ativa interrupção
	NVIC_SetPriority(PIT_IRQn, 3);
	NVIC_ClearPendingIRQ(PIT_IRQn); //limpa qlqr interrupção pendente do pit 
	NVIC_EnableIRQ(PIT_IRQn);		//habilita interrupção
}

void setup_PIT0_modoManual(){
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; //habilita o modulo pit
	PIT->MCR &= ~PIT_MCR_MDIS_MASK; //habilita o timer
	PIT->CHANNEL[0].LDVAL =10485760;   //1seg = 1*10485760
	PIT->CHANNEL[0].TCTRL &= PIT_TCTRL_CHN_MASK;//nao encadeia os timers
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;  //habilita o contador
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK; //ativa interrupção
	NVIC_SetPriority(PIT_IRQn, 3); 
	NVIC_ClearPendingIRQ(PIT_IRQn); //limpa qlqr interrupção pendente do pit 
	NVIC_EnableIRQ(PIT_IRQn);	//habilita interrupção
}

void desliga_PIT1(){
	PIT->CHANNEL[1].TCTRL = 0; //desabilita tudo no TCTRL
}
void desliga_PIT0(){
	PIT->CHANNEL[0].TCTRL = 0; //desabilita tudo no TCTRL
}