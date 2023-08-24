#include <stdint.h>
#include <MKL25Z4.h>
#include "teclado.h"


#define bounce 8

char teclado [4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

char teclaDeboucing( char bitColuna, char indColuna, char indLinha)
{
	char c;
	for (c = 0; c < bounce; c++)
	{
		atraso(1, 'm');
		if ((GPIOA_PDIR & (1 << bitColuna)) == 1)//caso a tecla seja lida como n?o precionada, repetimos a contagem, ela tem que se manter precionada por 100 contagens para ser considerada instavel e fora do per?odo de Bouncing
		{
			c = 0;
		}
	}
	return teclado[indLinha][indColuna];//numero da tecla precionada
}

char ligaLinhas(char bitColuna, char indColuna)//coluna = numero correspondente da coluna
{
	GPIOC_PSOR  |= (1 << 0 | 1 << 3 | 1 << 4);//deixamos apenas a primeira linha como nivel logico 0 para testear apenas ela  

	if ((GPIOA_PDIR & (1 << bitColuna)) == 0){
		return teclaDeboucing(bitColuna, indColuna, 0);
	}

	GPIOC_PSOR  |= (1 << 7);
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
	GPIOC_PCOR |= (1 << 7 | 1 << 0 | 1 << 3 | 1 << 4);
	
	while(!((GPIOA_PDIR & (1 << 1)) && (GPIOA_PDIR & (1 << 2)) && (GPIOA_PDIR & (1 << 4)) && (GPIOA_PDIR & (1 << 5))));

	while((GPIOA_PDIR & (1 << 1)) && (GPIOA_PDIR & (1 << 2)) && (GPIOA_PDIR & (1 <<4)) && (GPIOA_PDIR & (1 << 5)));//espera qualquer tecla ser pressionada

	return procuraLinhas();
}
