#include "validacao_senhas.h"
#include "lcdio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define senhaAdm "082479"
#define senhaOp1 "126963"
#define senhaOp2 "082479"


int nr_digitados = 0;
char senha[7] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0'};
const char* validacao;	
char admLogin = 0;

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

void digitarSenha(char teclaInterrupcao)
{		
	if ((teclaInterrupcao == '1') || (teclaInterrupcao == '2') || (teclaInterrupcao == '3') || (teclaInterrupcao == '4') || (teclaInterrupcao == '5') || (teclaInterrupcao == '6') || (teclaInterrupcao == '7') || (teclaInterrupcao == '8') || (teclaInterrupcao == '9') || (teclaInterrupcao == '0')){
			if(nr_digitados < 6){//caso a senha seja para desligar o alarme// digitos numericos somam para resultar em uma senha
				lendo_senha(teclaInterrupcao);
				send_data(teclaInterrupcao);
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
