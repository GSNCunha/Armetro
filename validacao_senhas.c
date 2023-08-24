#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcdio.h"
#include "validacao_senhas.h"


#define senhaAdm "082479"
#define senhaOp1 "126963"
#define senhaOp2 "082479"


int nr_digitados = 0;
char senha[7] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0'};
const char* validacao;	
char admLogin = 0;
char tecla;

void lendo_senha(char tecla){
			
	senha[nr_digitados] = tecla;
			
	//envia asterisco
	send_data(0x2A);
	nr_digitados++;
	
}


const char* validar_senha(void){ //compara senha digitada com senhas guardadas no sistema
	if(!strcmp(senha,senhaAdm)){
		return "adm";
	}else if(!strcmp(senha,senhaOp1)){
		return "operario1";
	}else if(!strcmp(senha,senhaOp2)){
		return "operario2";
	}else{
		return "senha incorreta";// se nao for nenhuma das anteriores, e uma senha invalida
	}
}

int resultado_validacao(void){
	
	validacao = validar_senha();
	
	if(!strcmp(validacao,"senha incorreta")){
		limpa_reseta_cursor();
		nr_digitados = 0;
		//telaSenhaIncorreta();
		return 0;//caso incorreta, o sistema nao permite acoes e usuario e pede novamente uma senha
	}else{
		limpa_reseta_cursor();
		nr_digitados = 0;
		//telaSenhaCorreta();
		//delay
		
		if (strcmp(validacao, "adm") == 0){
			//telaBemVindoAdm();
			//delay
			admLogin = 1;
			return 2; // p nao ativar o alarme quando for adm
		}
		return 1;
	}	
}

void digitarSenha()
{
		tecla = procuraTecla();
		
	if ((tecla == '1') || (tecla == '2') || (tecla == '3') || (tecla == '4') || (tecla == '5') || (tecla == '6') || (tecla == '7') || (tecla == '8') || (tecla == '9') || (tecla == '0')){
			if(nr_digitados < 6){//caso a senha seja para desligar o alarme// digitos numericos somam para resultar em uma senha
				lendo_senha(tecla);
			}
			if(nr_digitados == 6){//caso a senha tenha sido digitada por inteiro, vamos conferir seu resultado
				char result_val = resultado_validacao();
				if (result_val == 1){
					telaSenhaCorreta();
				}else{
				send_string("teu cu");
				}
			}
		}
}
