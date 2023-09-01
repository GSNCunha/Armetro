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
	/*SIM_SCGC6 |= 1<<23; //ativa PIT
	PIT_MCR &= ~(1 << 30); // 0 aqui pra habilitar o clock
	PIT_LDVAL0 = 500;//125829120; //valor inicial pro timer, ele conta até 0 e gera interrupção //12segundos = 125829120
	//valor atual = PIT_CVAL0;
	PIT_TCTRL0 &= (1 << 29);
	PIT_TCTRL0 |= (1 << 31); // 1<<30 ativa interrupcao, 1<<31 ativa o timer
	PIT_TCTRL0 |= (1 << 30);
	NVIC_SetPriority(PIT_IRQn, 3); 
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_EnableIRQ(PIT_IRQn);
	//NVIC_EnableIRQ();
	//__enable_irq();
	send_data('c');
	
	*/
	
	//Enable clock to PIT module
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
 //Enable module, freeze timers in debug mode
	PIT->MCR &= ~PIT_MCR_MDIS_MASK; //enable mdis
	
	//Initialize PIT0 to count down from starting_value
	PIT->CHANNEL[0].LDVAL =125829120;   //12seg
	//No chaining of timers
	PIT->CHANNEL[0].TCTRL &= PIT_TCTRL_CHN_MASK;
	
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
	
	//Let the PIT channel generate interrupt requests
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
	
	
	//NVIC_SetPriority(PIT_IRQn, 3); 
	//Clear any pending IRQ from PIT
	NVIC_ClearPendingIRQ(PIT_IRQn); 
	//Enable the PIT interrupt in the NVIC
	NVIC_EnableIRQ(PIT_IRQn);	


	
}
void setup_PIT1(){// led
	/*SIM_SCGC6 |= 1<<23; //ativa PIT
	PIT_MCR &= ~(1 << 30); // 0 aqui pra habilitar o clock
	PIT_MCR |= (1 << 31);
	PIT_LDVAL1 = 2621440; //valor inicial pro timer, ele conta até 0 e gera interrupção //3segundos = 31457280
	//valor atual = PIT_CVAL1;
	PIT_TCTRL1 = (1 << 30) | (1 << 31); // 1<<30 ativa interrupcao, 1<<31 ativa o timer
	*/
	
	
	//Enable clock to PIT module
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
 //Enable module, freeze timers in debug mode
	PIT->MCR &= ~PIT_MCR_MDIS_MASK; //enable mdis
	
	//Initialize PIT0 to count down from starting_value
	PIT->CHANNEL[1].LDVAL =2621440;   //timer do blink
	//No chaining of timers
	PIT->CHANNEL[1].TCTRL &= PIT_TCTRL_CHN_MASK;
	
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TEN_MASK;
	
	//Let the PIT channel generate interrupt requests
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TIE_MASK;
	
	
	//NVIC_SetPriority(PIT_IRQn, 3); 
	//Clear any pending IRQ from PIT
	NVIC_ClearPendingIRQ(PIT_IRQn); 
	//Enable the PIT interrupt in the NVIC
	NVIC_EnableIRQ(PIT_IRQn);	
	
	
}

void desliga_PIT1(){
	PIT->CHANNEL[1].TCTRL = 0;
}
void desliga_PIT0(){
	PIT->CHANNEL[0].TCTRL = 0;
}

void soma5_PIT0(){
	

}

