#include "timers.h"
#include "teclado.h"
#include "lcdio.h"
#include <stdint.h>
#include <MKL25Z4.h>

#define bounce 8//numero de testes para completar debounce

char tecla;

char teclado [4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

char teclaDeboucing( char bitColuna, char indColuna, char indLinha)
{
	char c;
	for (c = 0; c < bounce; c++)//só segue se a tecla estabilizar na tecla pressionada, precisa se manter estavel durante "bounce" testes.
	{
		atraso(1, 'm');
		if ((GPIOA_PDIR & (1 << bitColuna)) == 1)//caso a tecla seja lida como n?o precionada, repetimos a contagem, ela tem que se manter precionada por 100 contagens para ser considerada instavel e fora do per?odo de Bouncing
		{
			c = 0;
		}
	}
	GPIOC_PCOR |= (1 << 7 | 1 << 0 | 1 << 3 | 1 << 4); // seta todas as portas para zero para proxima leitura
	return teclado[indLinha][indColuna];//numero da tecla pressionada
}

char ligaLinhas(char bitColuna, char indColuna)//coluna = numero correspondente da coluna
{
	GPIOC_PSOR  |= (1 << 0 | 1 << 3 | 1 << 4);//deixamos apenas a primeira linha como nivel logico 0 para testar apenas ela  

	if ((GPIOA_PDIR & (1 << bitColuna)) == 0){
		return teclaDeboucing(bitColuna, indColuna, 0);
	}

	GPIOC_PSOR  |= (1 << 7);//precisamos desligar a ultima linha posta em 0 e ligar a proxima que queremos testar 
	GPIOC_PCOR |= (1 << 0);

	if ((GPIOA_PDIR & (1 << bitColuna)) == 0){
		return teclaDeboucing(bitColuna, indColuna, 1);
	}

	GPIOC_PSOR  |= (1 << 0);
	GPIOC_PCOR |= (1 << 3);

	if ((GPIOA_PDIR & (1 << bitColuna)) == 0){
		return teclaDeboucing(bitColuna, indColuna, 2);
	}

	GPIOC_PSOR  |= (1 << 3);
	GPIOC_PCOR |= (1 << 4);
	
	if ((GPIOA_PDIR & (1 << bitColuna)) == 0){
		return teclaDeboucing(bitColuna, indColuna, 3);
	}
	
	return '\0';
}

char procuraLinhas(void)
{
	if((GPIOA_PDIR & (1 << 1)) == 0)//coluna 1
	{
		return ligaLinhas(1, 0);
	}
	if((GPIOA_PDIR & (1 << 2)) == 0)//coluna 2
	{
		return ligaLinhas(2, 1);
	}
	if((GPIOA_PDIR & (1 << 4)) == 0)//coluna 3
	{
		return ligaLinhas(4, 2);
	}
	if((GPIOA_PDIR & (1 << 5)) == 0)//coluna 4
	{
		return ligaLinhas(5, 3);
	}
	return '\0';
}

char procuraTecla(void)
{
	
	/*
	nesse codigo basicamente botamos todas as linhas em 0, depois vemos se alguma coluna esta resposondendo em 0 tambem
	caso esteja respondendo, vamos ligando linha ppor linha e conferimos essa mesma coluna, depois de achar que linha que 
	causou a resposta dessa coluna em especifico, fazemos um debouncing e retornamos a funcao procuraTecla com a tecla pressionada.
	
	*/
	GPIOC_PCOR |= (1 << 7 | 1 << 0 | 1 << 3 | 1 << 4);

	return procuraLinhas();
}

char lerTeclado(){
	tecla = 0;
	while(1){
		tecla = procuraTecla();
		if(tecla == '1' || tecla == '2' || tecla == '3' || tecla == '4' || tecla == '5' || tecla == '6' || tecla == '7' || tecla == '8' || tecla == '9' || tecla == '0'){
			atraso(1, 's');
			return tecla;
		}
	}
}