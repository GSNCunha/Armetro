	#include <MKL25Z4.h>
	#include "lcdio.h"
	#include "timers.h"
	
	// RS: PTC8 (0: comando,  1: dados)
	// Enable: PTC9 

	void trigger_EN(void)
	{
		
		GPIOC_PSOR |= (1 << 9);//coloca HIGH
		atraso(3, 'm');
		
		GPIOC_PCOR |= (1 << 9);//coloca LOW
		atraso(3, 'm');
	}

	void send_data(char _data_) //Enviar dados
	{
		
		//RS = dados
		GPIOC_PSOR |= (1 << 8);
		atraso(3, 'm');
		
		GPIOD_PCOR = 0b11111111;
		atraso(3, 'm');
		
		GPIOD_PSOR = _data_;
		trigger_EN();
		
		atraso(3, 'm');
	}

	void send_command(char command) //enviar comando para o Hitachi
	{
		
		//RS = comandos
		GPIOC_PCOR |= (1 << 8);
		atraso(3, 'm');
		
		GPIOD_PCOR = 0b11111111;
		atraso(3, 'm');
		
		GPIOD_PSOR = command;
		trigger_EN();
		
		atraso(3, 'm');

	}

	void send_string(char *string){//Envia uma string de qualquer tamanho
		
		while(*string != 0){
			send_data(*string);
			string++;
		}
	}
	
	
	void limpa_reseta_cursor(void){
		//Limpa display e reseta posicao do cursor
		send_command(0x01);
		atraso(1, 's');
	}
	
	void config_lcd_padrao(void){
		
		//Primeiro comando tem que ser em 8 bits
		
		//FUNCTION SET:
		//Interface 8 bits
		//2 linhas
		//Caracteres 5x10
		send_command(0x38);
		
		//ENTRY MODE SET:
		//Incrementa posicao do cursor
		send_command(0x06);
		
		
		//CURSOR/DISPLAY SHIFT:
		//Cursor move
		//Para direita
		send_command(0x14);
		
		
		//DISPLAY CONTROL:
		//Display ligado
		//Cursor ligado
		//Cursor nao pisca
		send_command(0x0E);
		
	}
	
	void proxima_linha(void){
		send_command(0xC0);
		atraso(3, 'm');
	}
	
	
	/////// TELAS /////////////
	
	
	
	void telaArmMetro()
	{
		limpa_reseta_cursor();
		send_string("   ARMETRO    ");
		proxima_linha();
	}
	
		void telaSenhaCorreta(){
		limpa_reseta_cursor();
		send_string("SENHA CORRETA");
	}
		
		

