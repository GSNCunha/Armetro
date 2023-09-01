#include <MKL25Z4.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcdio.h"
#include "timers.h"
#include "teclado.h"
#include "validacao_senhas.h"

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
	
	
	//telas login:
	void telaArmMetro()
	{
		limpa_reseta_cursor();
		send_string("   ARMETRO    ");
		proxima_linha();
		send_string("SENHA: ");
	}
	void telaSenhaCorreta(){
		limpa_reseta_cursor();
		send_string("SENHA CORRETA");
		atraso(2, 's');
	}
	void telaSenhaIncorreta(){
		limpa_reseta_cursor();
		send_string("SENHA INCORRETA");
		atraso(2, 's');
	}
	
	void telaAdmLogado(){
		limpa_reseta_cursor();
		send_string("BEM-VINDO ADM");
		atraso(2, 's');
	}	

	//telas edição de parametros:
	void telaEditarParametros(){
		limpa_reseta_cursor();
		send_string("1-DIST  2-TEMPO");
		proxima_linha();
		send_string("3-VEL.MAX 4-SAIR");
	}
	
	void telaEditarDist(){
		limpa_reseta_cursor();
		send_string("1-EST.1 2-EST.2");
		proxima_linha();
		send_string("3-EST.3");
	}

	void telaEditarTempos(){
		limpa_reseta_cursor();
		send_string("1-VIAGEM 2-PARADA");
		proxima_linha();
	}
	
	void telaEditarTempoViagem(){
		limpa_reseta_cursor();
		send_string("1-EST.1 2-EST.2");
		proxima_linha();
		send_string("3-EST.3");
	}
	
	void telaEditarTempoParada(){
		limpa_reseta_cursor();
		send_string("1-EST.1 2-EST.2");
		proxima_linha();
		send_string("3-EST.3");
	}
	
	void telaEditarVel(){
		limpa_reseta_cursor();
		send_string("1-EST.1 2-EST.2");
		proxima_linha();
		send_string("3-EST.3");
	}
	void telaNovaVel(){
		limpa_reseta_cursor();
		send_string("NOVA VEL.MAX:");
		proxima_linha();
	}
	void telaNovaDist(){
		limpa_reseta_cursor();
		send_string("NOVA DIST:");
		proxima_linha();
	}
	
	void telaTempoParada(){
	
		limpa_reseta_cursor();
		send_string("NOVO TEMPO P.:");
		proxima_linha();
	
	}
	
	void telaTempoViagem(){
	
		limpa_reseta_cursor();
		send_string("NOVO TEMPO V.:");
		proxima_linha();
	
	}
	
	//telas pós login do maquinista:
	void telaModosMaquinista(){
		limpa_reseta_cursor();
		send_string("1 - AUTOMATICO");
		proxima_linha();
		send_string("2 - MANUAL");
	}
	